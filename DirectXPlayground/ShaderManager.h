#pragma once
#include "pch.h"
#include "Globals.h"
#include "ConstantBuffers.h"
#include "Shaders/IVertexShader.h"
#include "Shaders/TextureShader.h"

#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <streambuf>
#include <d3dcompiler.h>
#include <string>

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace DirectX;
using namespace Platform;
using namespace Windows::System;

// this function loads a file into an Array^
Array<byte>^ LoadShaderFile(std::string file);

class ShaderManager {
private:
	std::map<Shaders, IVertexShader*> m_vertexShaders{};
	std::map<Shaders, ComPtr<ID3D11PixelShader>> m_pixelShaders{};

private:
	void AddVertexShader(Shaders key, std::string path, ComPtr<ID3D11Device1> dev, int constBufSize) {
		Array<byte>^ vsFile = LoadShaderFile(path);
		ComPtr<ID3D11VertexShader> vertexShader = {};
		dev->CreateVertexShader(vsFile->Data, vsFile->Length, nullptr, vertexShader.GetAddressOf());
		
		TextureVS* vs = new TextureVS();
		vs->m_directXShaderObj = vertexShader;
		vs->m_vsFile = vsFile;
		vs->m_constBufferSize = constBufSize; // todo determine this based on Vertex Shader key
		vs->m_key = key;
		m_vertexShaders[key] = vs;
	}

	// todo should use and split up into SetupVS and AddVS or something...
	void SetupAndAddVertexShader(Shaders key, std::string path, ComPtr<ID3D11Device1> dev, IVertexShader* vs, int constBufSize) {
		Array<byte>^ vsFile = LoadShaderFile(path);
		ComPtr<ID3D11VertexShader> vertexShader = {};
		dev->CreateVertexShader(vsFile->Data, vsFile->Length, nullptr, vertexShader.GetAddressOf());

		vs->m_directXShaderObj = vertexShader;
		vs->m_vsFile = vsFile;
		vs->m_constBufferSize = constBufSize; // todo determine this based on Vertex Shader key
		vs->m_key = key;

		m_vertexShaders[key] = vs;
	}

	void AddPixelShader(Shaders key, std::string path, ComPtr<ID3D11Device1> dev) {
		Array<byte>^ psFile = LoadShaderFile(path);
		ComPtr<ID3D11PixelShader> pixelShader = {};
		dev->CreatePixelShader(psFile->Data, psFile->Length, nullptr, pixelShader.GetAddressOf());
		m_pixelShaders[key] = pixelShader;
	}

public:
	std::map<Shaders, std::vector<Entity*>> m_shadersMap{};

public:
	ShaderManager(ComPtr<ID3D11Device1> dev) {

		// load shader files (.hlsl files become .cso files after compilation)
		SetupAndAddVertexShader(Shaders::Texture, "TextureVS.cso", dev, new TextureVS(), sizeof(CONSTANTBUFFER));
		AddPixelShader(Shaders::Texture, "TexturePS.cso", dev);

		AddVertexShader(Shaders::ShinyMat, "ShinyMatVS.cso", dev, sizeof(SHINYMATCONSTBUFF));
		AddPixelShader(Shaders::ShinyMat, "ShinyMatPS.cso", dev);
	}

	void AddEntityToShaders(Shaders shader, Entity* entity) {
		m_shadersMap[shader].push_back(entity);
	}

	IVertexShader* GetVertexShader(Shaders key) { return m_vertexShaders[key]; };
	ComPtr<ID3D11PixelShader> GetPixelShader(Shaders key) { return m_pixelShaders[key]; };
};