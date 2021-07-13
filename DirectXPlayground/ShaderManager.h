#pragma once
#include "pch.h"
#include "Globals.h"
#include "ConstantBuffers.h"

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

struct IShader {
	ComPtr<ID3D11VertexShader> m_directXShaderObj;
	int m_constBufferSize;

	virtual const void* GetTextureVSConstBufferVals(XMMATRIX matFinal, XMMATRIX rot) = 0;
	virtual D3D11_BUFFER_DESC GetConstBufferDesc() = 0;
};

struct VertexShader : IShader {
	Shaders m_key;
	Array<byte>^ m_vsFile;

	D3D11_BUFFER_DESC GetConstBufferDesc() override {
		D3D11_BUFFER_DESC vsbd = { 0 };
		vsbd.Usage = D3D11_USAGE_DEFAULT;

		// constant buffers MUST be multiples of 16 bytes. If our constant buffer isn't a multiple of 16, the leftover bytes will be ignored
		vsbd.ByteWidth = m_constBufferSize;
		vsbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		return vsbd;
	}

	// todo build this according to VertexShaders key
	std::vector<D3D11_INPUT_ELEMENT_DESC> GetInputLayout() {
		std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
			// 5th param specifies on which byte the new piece of info starts
			// so position starts on byte 0, next on byte 12 etc
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT , 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT , 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		return ied;
	}

	// todo move out... but to where?
	// probably to Entity, since there I'll have access to all the info needed
	// to create matFinal, rotation, etc
	const void* GetTextureVSConstBufferVals(XMMATRIX matFinal, XMMATRIX rot) override {
		CONSTANTBUFFER* constBufVals = new CONSTANTBUFFER();

		constBufVals->matFinal = matFinal;
		constBufVals->rotation = rot;
		constBufVals->ambientColor = XMVectorSet(0.4f, 0.4f, 0.4f, 1.0f); // the higher the RGB values, the brighter the light
		constBufVals->diffuseColor = XMVectorSet(0.5f, 0.5f, 0.5f, 1.0f);
		constBufVals->diffuseVector = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);

		return constBufVals;
	}
};

class ShaderManager {
private:
	std::map<Shaders, VertexShader*> m_vertexShaders{};
	std::map<Shaders, ComPtr<ID3D11PixelShader>> m_pixelShaders{};

private:
	void AddVertexShader(Shaders key, std::string path, ComPtr<ID3D11Device1> dev, int constBufSize) {
		Array<byte>^ vsFile = LoadShaderFile(path);
		ComPtr<ID3D11VertexShader> vertexShader = {};
		dev->CreateVertexShader(vsFile->Data, vsFile->Length, nullptr, vertexShader.GetAddressOf());
		
		VertexShader* vs = new VertexShader();
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
		AddVertexShader(Shaders::Texture, "TextureVS.cso", dev, sizeof(CONSTANTBUFFER));
		AddPixelShader(Shaders::Texture, "TexturePS.cso", dev);

		AddVertexShader(Shaders::ShinyMat, "ShinyMatVS.cso", dev, sizeof(SHINYMATCONSTBUFF));
		AddPixelShader(Shaders::ShinyMat, "ShinyMatPS.cso", dev);
	}

	void AddEntityToShaders(Shaders shader, Entity* entity) {
		m_shadersMap[shader].push_back(entity);
	}

	VertexShader* GetVertexShader(Shaders key) { return m_vertexShaders[key]; };
	ComPtr<ID3D11PixelShader> GetPixelShader(Shaders key) { return m_pixelShaders[key]; };
};