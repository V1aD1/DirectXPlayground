#pragma once

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace DirectX;
using namespace Platform;

// used to mirror our constant buffer
struct CONSTANTBUFFER {

	// var names don't have to match with constant buffer BUT
	// variable size and order must match;
	
	// final transformation
	XMMATRIX final; // bytes 0 - 15 
	XMMATRIX rotation;
	//float padding1; // bytes 12 - 15 
	
	XMVECTOR diffuseVector;
	XMVECTOR diffuseColor;
	XMVECTOR ambientColor;
};

struct VERTEX {
	float X, Y, Z;
	float NX, NY, NZ;
};

class CGame {
public:
	// device interface
	ComPtr<ID3D11Device1> m_dev;

	//device context interface
	ComPtr<ID3D11DeviceContext1> m_devCon;

	// swap chain interface
	ComPtr<IDXGISwapChain1> m_swapChain;

	// COM object that maintains location in video memory to render to (usually back buffer)
	ComPtr<ID3D11RenderTargetView> m_renderTarget;

	// used to access video memory by maintaining both system and video memory
	// this object automatically copies data from system to video memory when necessary
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	
	ComPtr<ID3D11Buffer> m_indexBuffer;

	ComPtr<ID3D11Buffer> m_constantBuffer;

	ComPtr<ID3D11DepthStencilView> m_zBuffer;

	ComPtr<ID3D11VertexShader> m_vertexShader; // ran once for each vertex that gets rendered
	ComPtr<ID3D11PixelShader> m_pixelShader; // ran once for each pixel that gets drawn
	ComPtr<ID3D11InputLayout> m_inputLayout; // the input layout tells the GPU how our vertex struct is laid out

	void Initialize();
	void Update();
	void Render();

	void InitGraphics();
	void InitPipeline(); // initializes GPU settings and prepares it for rendering

	void PointerPressed();

private:
	CONSTANTBUFFER m_constBufferValues;
	float m_time;
};