#pragma once
#include "pch.h"
#include "Globals.h"

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

struct VertexShader {
	ComPtr<ID3D11VertexShader> m_directXShaderObj;
	Array<byte>^ m_vsFile;
};

class ShaderManager {
private:
	std::map<VertexShaders, VertexShader*> m_vertexShaders;
	std::map<PixelShaders, ComPtr<ID3D11PixelShader>> m_pixelShaders;

private:
	void AddVertexShader(VertexShaders key, std::string path, ComPtr<ID3D11Device1> dev) {
		Array<byte>^ vsFile = LoadShaderFile(path);
		ComPtr<ID3D11VertexShader> vertexShader = {};
		dev->CreateVertexShader(vsFile->Data, vsFile->Length, nullptr, vertexShader.GetAddressOf());
		
		VertexShader* vs = new VertexShader();
		vs->m_directXShaderObj = vertexShader;
		vs->m_vsFile = vsFile;
		m_vertexShaders[key] = vs;
	}

	void AddPixelShader(PixelShaders key, std::string path, ComPtr<ID3D11Device1> dev) {
		Array<byte>^ psFile = LoadShaderFile(path);
		ComPtr<ID3D11PixelShader> pixelShader = {};
		dev->CreatePixelShader(psFile->Data, psFile->Length, nullptr, pixelShader.GetAddressOf());
		m_pixelShaders[key] = pixelShader;
	}

public:
	ShaderManager(ComPtr<ID3D11Device1> dev) {

		// load shader files (.hlsl files become .cso files after compilation)
		AddVertexShader(VertexShaders::VertexShader1, "VertexShader.cso", dev);
		AddPixelShader(PixelShaders::PixelShader1, "PixelShader.cso", dev);
	}

	VertexShader* GetVertexShader(VertexShaders key) { return m_vertexShaders[key]; };
	ComPtr<ID3D11PixelShader> GetPixelShader(PixelShaders key) { return m_pixelShaders[key]; };
};