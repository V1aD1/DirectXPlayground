#include "pch.h"

#include "ShaderManager.h"
#include "Globals.h"
#include "ConstantBuffers.h"
#include "Shaders/IVertexShader.h"
#include "Shaders/TextureShader.h"
#include "Shaders/ShinyMatShader.h"

#include <string>
#include <fstream>
#include <streambuf>
#include <d3dcompiler.h>

ShaderManager::ShaderManager(ComPtr<ID3D11Device1> dev) {

	// load shader files (.hlsl files become .cso files after compilation)
	SetupAndAddVertexShader(Shaders::Texture, "TextureVS.cso", dev, new TextureVS(), sizeof(CONSTANTBUFFER));
	AddPixelShader(Shaders::Texture, "TexturePS.cso", dev);

	SetupAndAddVertexShader(Shaders::ShinyMat, "ShinyMatVS.cso", dev, new ShinyMatVS(), sizeof(SHINYMATCONSTBUFF));
	AddPixelShader(Shaders::ShinyMat, "ShinyMatPS.cso", dev);
}

void ShaderManager::SetupAndAddVertexShader(Shaders key, std::string path, ComPtr<ID3D11Device1> dev, IVertexShader* vs, int constBufSize) {
	Array<byte>^ vsFile = LoadShaderFile(path);
	ComPtr<ID3D11VertexShader> vertexShader = {};
	dev->CreateVertexShader(vsFile->Data, vsFile->Length, nullptr, vertexShader.GetAddressOf());

	vs->m_directXShaderObj = vertexShader;
	vs->m_vsFile = vsFile;
	vs->m_constBufferSize = constBufSize; // todo determine this based on Vertex Shader key
	vs->m_key = key;

	m_vertexShaders[key] = vs;
}

void ShaderManager::AddPixelShader(Shaders key, std::string path, ComPtr<ID3D11Device1> dev) {
	Array<byte>^ psFile = LoadShaderFile(path);
	ComPtr<ID3D11PixelShader> pixelShader = {};
	dev->CreatePixelShader(psFile->Data, psFile->Length, nullptr, pixelShader.GetAddressOf());
	m_pixelShaders[key] = pixelShader;
}

void ShaderManager::AddEntityToShaders(Shaders shader, Entity* entity) {
	m_shadersMap[shader].push_back(entity);
}

IVertexShader* ShaderManager::GetVertexShader(Shaders key) { 
	return m_vertexShaders[key]; 
};

ComPtr<ID3D11PixelShader> ShaderManager::GetPixelShader(Shaders key) { 
	return m_pixelShaders[key]; 
};

const void* ShaderManager::GetShinyMatVSConstBufferVals(XMMATRIX matFinal, XMMATRIX rot, XMFLOAT3 camPos) {
	SHINYMATCONSTBUFF* constBufVals = new SHINYMATCONSTBUFF();
	constBufVals->matFinal = matFinal;
	constBufVals->rotation = rot;
	constBufVals->color = XMVectorSet(1.0f, 0.4f, 0.4f, 1.0f);
	constBufVals->camPos = camPos;

	return constBufVals;
}

const void* ShaderManager::GetTextureVSConstBufferVals(XMMATRIX matFinal, XMMATRIX rot) {
	CONSTANTBUFFER* constBufVals = new CONSTANTBUFFER();

	constBufVals->matFinal = matFinal;
	constBufVals->rotation = rot;

	// todo set directional light to be a global set of values, accessible from any shader (then do the same for point lights?)
	constBufVals->ambientColor = XMVectorSet(0.4f, 0.4f, 0.4f, 1.0f); // the higher the RGB values, the brighter the light
	constBufVals->diffuseColor = XMVectorSet(0.5f, 0.5f, 0.5f, 1.0f);
	constBufVals->diffuseVector = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);

	return constBufVals;
}