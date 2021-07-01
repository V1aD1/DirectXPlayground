#pragma once

#include <vector>

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace DirectX;
using namespace Platform;
using namespace Windows::System;

class InputHandler;
class Entity;
class ShaderManager;
class Camera;
struct CONSTANTBUFFER;

class CGame {
public:
	static ComPtr<ID3D11RasterizerState> s_defaultRasterState;
	static ComPtr<ID3D11RasterizerState> s_wireframeRasterState;
	static ComPtr<ID3D11DepthStencilState> s_depthEnabledStencilState;
	static ComPtr<ID3D11DepthStencilState> s_depthDisabledStencilState;

public:
	// debug info
	ComPtr<ID3D11Debug> m_debug;

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

	ComPtr<ID3D11Buffer> m_VSConstantBuffer;
	ComPtr<ID3D11Buffer> m_PSConstantBuffer;

	ComPtr<ID3D11DepthStencilView> m_zBuffer;

	ComPtr<ID3D11VertexShader> m_vertexShader; // ran once for each vertex that gets rendered
	ComPtr<ID3D11PixelShader> m_pixelShader; // ran once for each pixel that gets drawn
	ComPtr<ID3D11InputLayout> m_inputLayout; // the input layout tells the GPU how our vertex struct is laid out
	
	// used to control "states" of the rendering pipeline
	// "states" are single properties of a fixed portion of the pipeline,
	// which can be set by you, but not directly accessed or written to
	ComPtr<ID3D11RasterizerState> m_rasterizerState;
	ComPtr<ID3D11BlendState> m_blendState; // blend state interface used for handling transparency
	ComPtr<ID3D11ShaderResourceView> m_woodTex;
	ComPtr<ID3D11ShaderResourceView> m_bricksTex;
	ComPtr<ID3D11ShaderResourceView> m_grassTex;
	ComPtr<ID3D11SamplerState> m_samplerStates[1]; // sample state interfaces

	bool m_wireFrame;

public:
	void Initialize();
	void InitGraphics();
	void InitPipeline(); // initializes GPU settings and prepares it for rendering
	void InitStates();
	void AddEntitiesToWorld();

	void Update();
	void Render();

	void PointerPressed(PointerEventArgs^ args);
	void PointerReleased(PointerEventArgs^ args);
	void PointerMoved(PointerEventArgs^ args);
	void KeyDown(VirtualKey key);
	void KeyUp(VirtualKey key);

	void Finalize();

private:
	CONSTANTBUFFER* m_constBufferValues;
	float m_time{0};
	Entity* m_camera;
	std::vector<Entity*> m_entities{};
	InputHandler* m_inputHandler;
	ShaderManager* m_shaderManager;

	void AddTexture(const wchar_t* textName, ComPtr<ID3D11ShaderResourceView>& resToMapTo);
};