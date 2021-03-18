#include "pch.h"
#include "Game.h"

#include <string>
#include <fstream>

// this function loads a file into an Array^
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
void CGame::Initialize(){
	// define temp pointers to a device and device context
	ComPtr<ID3D11Device> dev11;
	ComPtr<ID3D11DeviceContext> devcon11;

	// create the device and device context objects
	D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
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
	textDesc.SampleDesc.Count = 1; // anti aliasing kindof, disabled the stair-effect on diagonal lines
	textDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // 24 bits for depth, 8 bits for stencil 
	textDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL; // flag specifies that this texture will be used as a depth buffer
	
	ComPtr<ID3D11Texture2D> zBufferTexture;
	m_dev->CreateTexture2D(&textDesc, nullptr, &zBufferTexture);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; // specifies what kind of texture we're using
	m_dev->CreateDepthStencilView(zBufferTexture.Get(), &dsvd, &m_zBuffer);

	m_devCon->OMSetRenderTargets(1, m_renderTarget.GetAddressOf(), m_zBuffer.Get());

	// set the viewport (an object that describes what part of the back buffer to draw on)
	D3D11_VIEWPORT viewport = { 0 };
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = window->Bounds.Width;
	viewport.Height = window->Bounds.Height;
	viewport.MinDepth = 0; // closest an object can be on the depth buffer is 0.0
	viewport.MaxDepth = 1; // farthest an object can be on the depth buffer is 1.0

	m_devCon->RSSetViewports(1, &viewport);

	// initialize graphics and pipeline
	InitGraphics();
	InitPipeline();
	m_time = 0.0f;
}

// performs updates to the state of the game
void CGame::Update() {
	m_time += 0.05f;
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

	// set the vertex buffer
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	m_devCon->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	m_devCon->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	// set the primitive topology (remember that we're drawing a triangle)
	m_devCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// WORLD transformation
	XMMATRIX matTranslate = XMMatrixTranslation(0, 0, 0);
	XMMATRIX matRotateY = XMMatrixRotationY(XMConvertToRadians(m_time * 20));
	XMMATRIX matScale = XMMatrixIdentity();

	// order here matters! Most of the time you'll want your translation to go last!
	// XMMATRIX matWorld = matRotateY * matScale * matTranslate;

	// VIEW transformation
	float camPosX = 0;
	float camPosY = 6.0f;
	float camPosZ = 10.0f;
	XMVECTOR vecCamPosition = XMVectorSet(camPosX, camPosY, camPosZ, 0);
	XMVECTOR vecCamLookAt = XMVectorSet(0, 0, 0, 0);
	XMVECTOR vecCamUp = XMVectorSet(0, 1, 0, 0); // y axis is usually up for our camera
	XMMATRIX matView = XMMatrixLookAtLH(vecCamPosition, vecCamLookAt, vecCamUp);

	// PROJECTION transformation
	CoreWindow^ window = CoreWindow::GetForCurrentThread();
	XMMATRIX matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45), // the field of view
		(FLOAT)window->Bounds.Width / (FLOAT)window->Bounds.Height, // aspect ratio
		1, // the near view plane (usually 1)
		100 // the far view plane
	);

	XMMATRIX matFinal = matRotateY * matScale * matTranslate * matView * matProjection;

	// draw each triangle

	// setup the new values for the constant buffer
	//m_devCon->UpdateSubresource(m_constantBuffer.Get(), 0, 0, &m_constBufferValues, 0, 0);
	m_devCon->UpdateSubresource(m_constantBuffer.Get(), 0, 0, &matFinal, 0, 0);

	// draw 4 vertices onto the buffer, starting from vertex 0
	//m_devCon->Draw(4, 0);
	m_devCon->DrawIndexed(24, 0, 0);

	// switch the back buffer and the front buffer
	m_swapChain->Present(1, 0);
}

void CGame::InitGraphics()
{
	// currently draws a square
	VERTEX ourVertices[] =
	{
		{ -1.0f, 1.0f, -1.0f,  1.0f, 0.0f, 0.0f },    // vertex 0
		{ 1.0f, 1.0f, -1.0f,   0.0f, 1.0f, 0.0f },    // vertex 1
		{ -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f },    // 2
		{ 1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 1.0f },    // 3
		{ -1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 1.0f },    // ...
		{ 1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },
		{ -1.0f, -1.0f, 1.0f,  1.0f, 1.0f, 0.0f },
		{ 1.0f, -1.0f, 1.0f,   1.0f, 1.0f, 1.0f },
	};

	// struct specifying properties of the buffer
	D3D11_BUFFER_DESC bd = { 0 };

	// size of the buffer that we'll create, in bytes
	bd.ByteWidth = sizeof(VERTEX) * ARRAYSIZE(ourVertices);

	// what kind of buffer we're making (vertex buffer)
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// data we're going to store in the vertex buffer
	D3D11_SUBRESOURCE_DATA srd = { ourVertices, 0, 0 };

	m_dev->CreateBuffer(&bd, &srd, &m_vertexBuffer);

	short ourIndices[] = {
		0, 1, 2,    // side 1
		1, 3, 2,
		/*4, 0, 6,    // side 2
		6, 0, 2,
		7, 5, 6,    // side 3
		6, 5, 4,
		3, 1, 7,    // side 4
		7, 1, 5,
		4, 5, 0,    // side 5
		0, 5, 1,
		3, 7, 2,    // side 6
		2, 7, 6,*/
	};

	D3D11_BUFFER_DESC ibd = { 0 };
	ibd.ByteWidth = sizeof(short) * ARRAYSIZE(ourIndices); // indices are stored in short values
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	
	D3D11_SUBRESOURCE_DATA isrd = { ourIndices, 0, 0 };
	
	m_dev->CreateBuffer(&ibd, &isrd, &m_indexBuffer);

	m_constBufferValues.X = 0.0f;
	m_constBufferValues.Y = 0.0f;
	m_constBufferValues.Z = 1.0f;

	m_constBufferValues.R = 1.0f;
	m_constBufferValues.G = 1.0f;
	m_constBufferValues.B = 1.0f;
	m_constBufferValues.A = 1.0f;
}

void CGame::InitPipeline()
{
	// load shader files (.hlsl files become .cso files after compilation)
	Array<byte>^ vsFile = LoadShaderFile("VertexShader.cso");
	Array<byte>^ psFile = LoadShaderFile("PixelShader.cso");

	m_dev->CreateVertexShader(vsFile->Data, vsFile->Length, nullptr, &m_vertexShader);
	m_dev->CreatePixelShader(psFile->Data, psFile->Length, nullptr, &m_pixelShader);

	// set the shader objects as the active shaders
	m_devCon->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	m_devCon->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	// initialize input layout
	D3D11_INPUT_ELEMENT_DESC ied[] = {
		// 5th param specifies on which byte the new piece of info starts
		// so position starts on byte 0, color on byte 12
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT , 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// create the input layout
	m_dev->CreateInputLayout(ied, ARRAYSIZE(ied), vsFile->Data, vsFile->Length, &m_inputLayout);
	m_devCon->IASetInputLayout(m_inputLayout.Get());

	// create the constant buffer
	D3D11_BUFFER_DESC bd = { 0 };
	bd.Usage = D3D11_USAGE_DEFAULT;

	// constant buffers MUST be multiples of 16 bytes. if our constant buffer isn't a multiple of 16, the leftover bytes will be ignored
	bd.ByteWidth = 64;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	m_dev->CreateBuffer(&bd, nullptr, &m_constantBuffer);
	m_devCon->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
	m_devCon->PSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
}

void CGame::PointerPressed()
{
	m_constBufferValues.X = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	m_constBufferValues.Y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	m_constBufferValues.Z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	m_constBufferValues.R = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	m_constBufferValues.G = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	m_constBufferValues.B = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	m_constBufferValues.A = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}
