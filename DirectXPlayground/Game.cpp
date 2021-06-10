#include "pch.h"
#include "Game.h"
#include "math.h"
#include "WICTextureLoader.h"

#include <string>
#include <fstream>
#include <streambuf>
#include <d3dcompiler.h>

#include "ShaderManager.h"
#include "ConstantBuffer.h"
#include "Logger.h"

#include "Entities/Camera/CameraInputComponent.h"
#include "Entities/Camera/CameraPhysicsComponent.h"
#include "Entities/RotatingPhysicsComponent.h"
#include "Entities/InputHandler.h"
#include "Entities/Entity.h"
#include "Entities/Cube/CubeGraphicsComponent.h"

// definitions for static variables
ComPtr<ID3D11RasterizerState> CGame::s_defaultRasterState;
ComPtr<ID3D11RasterizerState> CGame::s_wireframeRasterState;
ComPtr<ID3D11DepthStencilState> CGame::s_depthEnabledStencilState;
ComPtr<ID3D11DepthStencilState> CGame::s_depthDisabledStencilState;

// todo move this to ShaderManager.h
Array<byte>^ LoadShaderFile(std::string file) {
	Array<byte>^ fileData = nullptr;

	//open the file
	std::ifstream vertexFile(file, std::ios::in | std::ios::binary | std::ios::ate);

	if (vertexFile.is_open()) {
		// get length of file
		int length = (int)vertexFile.tellg();

		// collect file data
		fileData = ref new Array<byte>(length);
		vertexFile.seekg(0, std::ios::beg);
		vertexFile.read(reinterpret_cast<char*>(fileData->Data), length);
		vertexFile.close();
	}

	return fileData;
}

// initializes and prepares Direct3D for use
void CGame::Initialize() {
	// define temp pointers to a device and device context
	ComPtr<ID3D11Device> dev11;
	ComPtr<ID3D11DeviceContext> devcon11;

	// create the device and device context objects
	D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&dev11,
		nullptr,
		&devcon11);

	// convert the pointers from the directx 11 versions to the directx 11.1 versions
	dev11.As(&m_dev);
	devcon11.As(&m_devCon);

	// creating swap chain requires a DXGI Factory object and 
	// a swap chain description

	// first convert our ID3D11Device1 into an IDGGIDevice1
	ComPtr<IDXGIDevice1> dxgiDevice;
	m_dev.As(&dxgiDevice);

	// second use the IDXGIDevice1 interface to get access to the adapter
	ComPtr<IDXGIAdapter> dxgiAdapter;
	dxgiDevice->GetAdapter(&dxgiAdapter);

	// third use the IDXGIAdapter interface to get access to the factory
	ComPtr<IDXGIFactory2> dxgiFactory;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), &dxgiFactory);

	HRESULT hr = m_dev->QueryInterface(IID_PPV_ARGS(&m_debug));

	// set up the swap chain description
	DXGI_SWAP_CHAIN_DESC1 scd = { 0 };
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // what the swap chain will be used for
	scd.BufferCount = 2; // 2 buffers, a front and back buffer
	scd.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // the most common swap chain format: 8 bits for R, G, B and Alpha each
	scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // recommended flip mode

	// disables anti-aliasing since not all gpu's support it, but maybe play with it later? There's also SampleDesc.Quality
	scd.SampleDesc.Count = 1;

	CoreWindow^ window = CoreWindow::GetForCurrentThread();

	dxgiFactory->CreateSwapChainForCoreWindow(
		m_dev.Get(), // address of the device
		reinterpret_cast<IUnknown*>(window), // address of the window
		&scd, // address of the swap chain description
		nullptr, // advanced stuff don't even WORRY about it! Pertains to which monitor will display the graphics
		&m_swapChain); // address of the new swap chain ptr

	// get a pointer directly to the back buffer
	ComPtr<ID3D11Texture2D> backBuffer;
	m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer);

	// create a render target pointing to the back buffer
	m_dev->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_renderTarget);

	D3D11_TEXTURE2D_DESC textDesc = { 0 };
	textDesc.Width = window->Bounds.Width; // z buffer size should match dimensions of window
	textDesc.Height = window->Bounds.Height;
	textDesc.ArraySize = 1; // how many textures to make
	textDesc.MipLevels = 1; // can be used to create multiple versions of a texture, each half the size of the previous texture
	textDesc.SampleDesc.Count = 1; // anti aliasing kindof, minimizes the stair-effect on diagonal lines
	textDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // 24 bits for depth, 8 bits for stencil 
	textDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL; // flag specifies that this texture will be used as a depth buffer

	ComPtr<ID3D11Texture2D> zBufferTexture;
	m_dev->CreateTexture2D(&textDesc, nullptr, &zBufferTexture);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; // specifies what kind of texture we're using
	m_dev->CreateDepthStencilView(zBufferTexture.Get(), &dsvd, &m_zBuffer);

	m_shaderManager = new ShaderManager(m_dev);

	// set the viewport (an object that describes what part of the back buffer to draw on)
	D3D11_VIEWPORT viewport = { 0 };
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = window->Bounds.Width;
	viewport.Height = window->Bounds.Height;
	viewport.MinDepth = 0; // closest an object can be on the depth buffer is 0.0
	viewport.MaxDepth = 1; // farthest an object can be on the depth buffer is 1.0

	m_devCon->RSSetViewports(1, &viewport);

	RECT scissor;
	scissor.left = 0;
	scissor.top = 0;
	scissor.right = window->Bounds.Width;
	scissor.bottom = window->Bounds.Height / 2; // half the height of the window
	m_devCon->RSSetScissorRects(1, &scissor); // doesn't work unless scissoring is enabled through rasterizer state

	m_constBufferValues = new CONSTANTBUFFER();
	InitGraphics();
	InitPipeline();
	InitStates();
	AddEntitiesToWorld();
	m_wireFrame = false;
	m_time = 0.0f;
	m_inputHandler = new InputHandler();
	m_camera->m_physics->SetPosition(Vector3(0.0f, 6.0f, 40.0f));
}

void CGame::InitGraphics()
{
	AddTexture(L"bricks.png", m_texture2);
	AddTexture(L"wood.png", m_texture1);
}

void CGame::InitPipeline()
{
}

void CGame::InitStates()
{
	/*
	rd.FillMode = D3D11_FILL_WIREFRAME; // render in wire frame mode
	rd.CullMode = D3D11_CULL_FRONT; // don't draw the front of primitves, only the back
	rd.FrontCounterClockwise = TRUE; // set the front to counter-clockwise
	rd.DepthClipEnable = FALSE; // forces depth of each pixel to be between two planes, but remember to set viewport.MaxDepth to 0.99f!
	rd.ScissorEnable = TRUE;
	rd.AntialiasedLineEnable = TRUE;
	rd.MultisampleEnable = FALSE;
	rd.DepthBias = 0; // this value is added to the depth of any pixel rendered
	rd.DepthBiasClamp = 0.0f; // limits the amount of depth bias so that certain distortions are avoided
	rd.SlopeScaledDepthBias = 0.0f; //this state is used to create high quality shadows (later)
	*/

	// set rasterizer properties
	D3D11_RASTERIZER_DESC rd;
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_BACK;
	rd.FrontCounterClockwise = FALSE;
	rd.DepthClipEnable = TRUE;
	rd.ScissorEnable = FALSE;
	rd.AntialiasedLineEnable = FALSE;
	rd.MultisampleEnable = FALSE;
	rd.DepthBias = 0;
	rd.DepthBiasClamp = 0.0f;
	rd.SlopeScaledDepthBias = 0.0f;
	m_dev->CreateRasterizerState(&rd, &s_defaultRasterState);

	rd.FillMode = D3D11_FILL_WIREFRAME;
	rd.AntialiasedLineEnable = TRUE;
	m_dev->CreateRasterizerState(&rd, &s_wireframeRasterState);

	// used for transparent objects (which should be drawn AFTER all non transparent objects are drawn )
	// blending equation: (Source Color * Source Factor (usually source alpha)) OPERATION (Dest Color * Dest Factor (don't know what it usually is))
	D3D11_BLEND_DESC bd;
	bd.RenderTarget[0].BlendEnable = TRUE; // turn on color blending
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // use source alpha for source factor
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // use inverse source alpha for dest factor

															  // blending alpha instead of colors (rarely needed, and disabled here)
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;    // addition is default for this
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;      // use full source alpha
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;    // use no dest alpha

	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // can be used to disable entire color channels (R, G or B)
	bd.IndependentBlendEnable = FALSE; // used to activate other render targets, but we only have 1, so we don't use this
	bd.AlphaToCoverageEnable = FALSE; // improves antialiasing of transparent textures

	m_dev->CreateBlendState(&bd, &m_blendState);

	D3D11_DEPTH_STENCIL_DESC dsd = { 0 };
	dsd.DepthEnable = TRUE; // can disable depth buffer
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; // can disable WRITING to the depth buffer
	dsd.DepthFunc = D3D11_COMPARISON_LESS; // only render pixels CLOSER to the camera than what's already on the depth buffer

	m_dev->CreateDepthStencilState(&dsd, &s_depthEnabledStencilState);

	dsd.DepthEnable = FALSE;
	m_dev->CreateDepthStencilState(&dsd, &s_depthDisabledStencilState);

	/*
	D3D11_SAMPLER_DESC sd;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.MaxAnisotropy = 8; // 1 <= value <= 16
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; // horizontally, the texture is repeated
	sd.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR; // vertically, the texture is mirrored
	sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP; // if it's a 3D texture, it get clamped
	sd.BorderColor[0] = 1.0f; // setting border color to white
	sd.BorderColor[1] = 1.0f;
	sd.BorderColor[2] = 1.0f;
	sd.BorderColor[3] = 1.0f;
	sd.MinLOD = 0.0f;
	sd.MaxLOD = FLT_MAX;
	sd.MipLODBias = 2.0f; // decrease mip level of detail by 2 all the time
	m_dev->CreateSamplerState(&sd, &m_samplerStates[0]);
	*/

	D3D11_SAMPLER_DESC sd;
	sd.Filter = D3D11_FILTER_ANISOTROPIC;
	sd.MaxAnisotropy = 16;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.BorderColor[0] = 1.0f;
	sd.BorderColor[1] = 1.0f;
	sd.BorderColor[2] = 1.0f;
	sd.BorderColor[3] = 1.0f;
	sd.MinLOD = 0.0f;
	sd.MaxLOD = FLT_MAX;
	sd.MipLODBias = 0.0f;
	m_dev->CreateSamplerState(&sd, &m_samplerStates[0]); // anisotropic sampler
}

void CGame::AddEntitiesToWorld()
{
	auto cube1Graphics = new CubeGraphicsComponent();
	cube1Graphics->AddTexture(m_texture1);
	cube1Graphics->AddTexture(m_texture2);
	auto cube1 = new Entity(NULL, new RotatingPhysicsComponent(), cube1Graphics);
	m_entities.push_back(cube1);

	auto cube2Physics = new PhysicsComponent(Vector3{ 4, 4, 0 }, Vector3{}, XMMatrixScaling(4, 2, 4));
	auto cube2Graphics = new CubeGraphicsComponent();
	cube2Graphics->AddTexture(m_texture1);
	cube2Graphics->AddTexture(m_texture2);
	auto cube2 = new Entity(NULL, cube2Physics, cube2Graphics);
	m_entities.push_back(cube2);

	m_camera = new Entity(new CameraInputComponent(), new CameraPhysicsComponent(), NULL);
	m_entities.push_back(m_camera);
}

// performs updates to the state of the game
void CGame::Update() {
	// todo fix with actual time passing
	auto dt = 0.02f;

	for (auto&& entity : m_entities) {
		entity->Update(dt, *m_inputHandler);
	}

	m_time += dt;
}

// renders a single frame of 3D graphics
void CGame::Render() {

	// set our render target object as the active render target
	m_devCon->OMSetRenderTargets(1, m_renderTarget.GetAddressOf(), m_zBuffer.Get());

	// clear the back buffer to a single color
	float color[4] = { 0.0f, 0.0f, 0.4f, 1.0f }; // R G B A
	m_devCon->ClearRenderTargetView(m_renderTarget.Get(), color);

	// clear the depth buffer to have all values set to 1.0f
	m_devCon->ClearDepthStencilView(m_zBuffer.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	// VIEW transformation
	XMVECTOR vecCamLookAt = XMVectorSet(0, 0, 0, 0);
	XMVECTOR vecCamLookTo = XMLoadFloat3(&(m_camera->m_physics->GetForwardDir()));
	XMVECTOR vecCamUp = XMVectorSet(0, 1, 0, 0); // y axis is usually up for our camera
	auto camPos = XMLoadFloat3(&(m_camera->m_physics->GetPosition()));
	
	// todo replace with using XMMatrixLookToLH() once I have a direction to look in
	//XMMATRIX matView = XMMatrixLookAtLH(camPos, vecCamLookAt, vecCamUp);
	XMMATRIX matView = XMMatrixLookToLH(camPos, vecCamLookTo, vecCamUp);

	// PROJECTION transformation
	CoreWindow^ window = CoreWindow::GetForCurrentThread();
	XMMATRIX matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45), // the field of view
		(FLOAT)window->Bounds.Width / (FLOAT)window->Bounds.Height, // aspect ratio
		1, // the near view plane (usually 1)
		100 // the far view plane
	);

	UINT stride = sizeof(VERTEX);
	UINT offset = 0;

	m_devCon->RSSetState(s_defaultRasterState.Get());
	m_devCon->OMSetBlendState(m_blendState.Get(), 0, 0xffffffff);
	m_devCon->OMSetDepthStencilState(s_depthEnabledStencilState.Get(), 0);
	m_devCon->PSSetSamplers(0, 1, m_samplerStates[0].GetAddressOf());

	// create the constant buffer
	D3D11_BUFFER_DESC bd = { 0 };
	bd.Usage = D3D11_USAGE_DEFAULT;

	// constant buffers MUST be multiples of 16 bytes. If our constant buffer isn't a multiple of 16, the leftover bytes will be ignored
	bd.ByteWidth = sizeof(CONSTANTBUFFER);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	m_dev->CreateBuffer(&bd, nullptr, &m_constantBuffer);

	m_devCon->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
	m_devCon->PSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
	m_devCon->RSSetState(m_rasterizerState.Get());

	// todo iterate over our shaders and render those entities
	// iterate over our entities and render them
	for (auto&& entity : m_entities) {
		if (!entity->m_graphics) { continue; }

		auto graphics = entity->m_graphics;
		VertexShader* vs = m_shaderManager->GetVertexShader(graphics->m_vertexShader);
		m_devCon->VSSetShader(vs->m_directXShaderObj.Get(), nullptr, 0);
		m_devCon->PSSetShader(m_shaderManager->GetPixelShader(graphics->m_pixelShader).Get(), nullptr, 0);

		for (int i = 0; i < graphics->m_textures.size(); i++) {
		m_devCon->PSSetShaderResources(i, 1, graphics->m_textures[i].GetAddressOf());
		}

		// initialize input layout
		D3D11_INPUT_ELEMENT_DESC ied[] = {
		// 5th param specifies on which byte the new piece of info starts
		// so position starts on byte 0, next on byte 12 etc
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT , 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT , 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		m_dev->CreateInputLayout(ied, ARRAYSIZE(ied), vs->m_vsFile->Data, vs->m_vsFile->Length, &m_inputLayout);
		m_devCon->IASetInputLayout(m_inputLayout.Get());

		// buffer work
		m_dev->CreateBuffer(&(graphics->m_vbDesc), &(graphics->m_vertexData), &m_vertexBuffer);
		m_dev->CreateBuffer(&(graphics->m_ibDesc), &(graphics->m_indexData), &m_indexBuffer);
		m_devCon->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
		m_devCon->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

		m_devCon->IASetPrimitiveTopology(graphics->m_topology);

		// order here matters! Most of the time you'll want translation AFTER rot and scale
		auto physics = entity->m_physics;
		XMMATRIX matFinal = physics->GetQuaternion() * physics->GetScale() * physics->GetTranslation() * matView * matProjection;

		// set constant buffer
		m_constBufferValues->matFinal = matFinal;
		m_constBufferValues->rotation = entity->m_physics->GetQuaternion();
		m_constBufferValues->ambientColor = XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f); // the higher the RGB values, the brighter the light
		m_constBufferValues->diffuseColor = XMVectorSet(0.5f, 0.5f, 0.5f, 1.0f);
		m_constBufferValues->diffuseVector = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
		m_devCon->UpdateSubresource(m_constantBuffer.Get(), 0, 0, m_constBufferValues, 0, 0);

		m_devCon->DrawIndexed(graphics->m_indices.size(), 0, 0);
	}

	// switch the back buffer and the front buffer
	m_swapChain->Present(1, 0);
}

void CGame::PointerPressed(PointerEventArgs^ args)
{
	m_inputHandler->OnMouseKeyChanged(args->CurrentPoint);
}

void CGame::PointerReleased(PointerEventArgs^ args)
{
	m_inputHandler->OnMouseKeyChanged(args->CurrentPoint);
}

void CGame::PointerMoved(PointerEventArgs^ args)
{
	auto currPoint = args->CurrentPoint;
	m_inputHandler->OnMouseMoved(args->CurrentPoint);
}

// todo create EventHandler, which will replace this method and contain flags for all keys and mouse stuff being pressed
//		in Update(), InputComponent should update and set states in GameObject(which will contain position, rotation and scale by default).
//		GameObject will also be made up of physics and graphics components, so InputComponent can set flags in PhysicsComponent by calling public functions
//		like physicsComp->SetBrakeFlag(), which will be unimplemented in GameObject, but MAY be overridden by specific PhysicsComponent
//		then PhysicsComponent should update according to its states, (Graphics component will update in Render()?)
void CGame::KeyDown(VirtualKey key)
{
	m_inputHandler->KeyDown(key);
}

void CGame::KeyUp(VirtualKey key)
{
	m_inputHandler->KeyUp(key);
}

void CGame::Finalize()
{
	Logger::Log("CGame::Finalize()");

	if (m_debug) {
		m_debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	}

	delete m_camera;
	delete m_inputHandler;
	delete m_shaderManager;

	for (auto entity : m_entities) {
		delete entity;
	}

	m_entities.clear();
}

void CGame::AddTexture(const wchar_t* textName, ComPtr<ID3D11ShaderResourceView>& resToMapTo)
{
	HRESULT hr = CreateWICTextureFromFile(m_dev.Get(), // our device 
		nullptr, // our device context but DON'T use it since it makes the function unstable!!
		textName, // name of file, with project folder as root file
		nullptr,
		&resToMapTo,
		0); // max size of texture, if 0 we load full texture
}
