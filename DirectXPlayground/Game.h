#pragma once

#include "GraphicsObject.h"
#include "ConstantBuffer.h"
#include "ShaderManager.h"

#include <map>

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace DirectX;
using namespace Platform;
using namespace Windows::System;

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

	ComPtr<ID3D11Buffer> m_constantBuffer;

	ComPtr<ID3D11DepthStencilView> m_zBuffer;

	ComPtr<ID3D11VertexShader> m_vertexShader; // ran once for each vertex that gets rendered
	ComPtr<ID3D11PixelShader> m_pixelShader; // ran once for each pixel that gets drawn
	ComPtr<ID3D11InputLayout> m_inputLayout; // the input layout tells the GPU how our vertex struct is laid out
	
	// used to control "states" of the rendering pipeline
	// "states" are single properties of a fixed portion of the pipeline,
	// which can be set by you, but not directly accessed or written to
	ComPtr<ID3D11RasterizerState> m_rasterizerState;
	ComPtr<ID3D11BlendState> m_blendState; // blend state interface used for handling transparency
	ComPtr<ID3D11ShaderResourceView> m_texture1;
	ComPtr<ID3D11ShaderResourceView> m_texture2;
	ComPtr<ID3D11SamplerState> m_samplerStates[2]; // sample state interfaces

	ShaderManager m_shaderManager;

	bool m_wireFrame;

	void Initialize();
	void InitGraphics();
	void InitPipeline(); // initializes GPU settings and prepares it for rendering
	void InitStates();
	void AddObjectsToWorld();

	void Update();
	void Render();

	void PointerPressed();
	void KeyPressed(VirtualKey key);

	void Finalize();

private:
	CONSTANTBUFFER m_constBufferValues;
	float m_time;
	XMVECTOR m_vecCamPosition;
	bool m_blurred;
	std::vector<GraphicsObject*> m_objects;
	std::map<VertexShaders, std::vector<GraphicsObject*>> m_vertexShaderMap;
	std::map<PixelShaders, std::vector<GraphicsObject*>> m_pixelShaderMap;

	// todo maybe split into GetBoxVertices(), SetVerticesToBuffers()?
	// todo create new class ObjectFactory?
	// todo create new class Object, with method AddToBuffers()?
	void AddBoxToBuffers();
	void AddTexture(const wchar_t* textName, ComPtr<ID3D11ShaderResourceView>& resToMapTo);
};