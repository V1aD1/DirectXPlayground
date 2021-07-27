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
	SetupAndAddVertexShader(ShaderKeys::Texture, "TextureVS.cso", std::shared_ptr<IVertexShader> (new TextureVS()), dev, sizeof(CONSTANTBUFFER_VS));
	AddPixelShader(ShaderKeys::Texture, "TexturePS.cso", std::shared_ptr<IPixelShader> (new TexturePS), dev, 0);

	SetupAndAddVertexShader(ShaderKeys::ShinyMat, "ShinyMatVS.cso", std::shared_ptr<IVertexShader>(new ShinyMatVS()), dev, sizeof(SHINYMATCONSTBUFF_VS));
	AddPixelShader(ShaderKeys::ShinyMat, "ShinyMatPS.cso", std::shared_ptr<IPixelShader>(new ShinyMatPS), dev, sizeof(SHINYMATCONSTBUFF_PS));
}

ShaderManager::~ShaderManager()
{
}

void ShaderManager::SetupAndAddVertexShader(ShaderKeys key, std::string path, std::shared_ptr<IVertexShader> vs, 
											ComPtr<ID3D11Device1> dev, int constBufSize) {
	Array<byte>^ vsFile = LoadShaderFile(path);
	ComPtr<ID3D11VertexShader> vertexShader = {};
	dev->CreateVertexShader(vsFile->Data, vsFile->Length, nullptr, vertexShader.GetAddressOf());

	vs->m_directXObjVS = vertexShader;
	vs->m_shaderFile = vsFile;
	vs->m_constBufferSize = constBufSize; // todo determine this based on Vertex Shader key
	vs->m_key = key;

	m_vertexShaders[key] = vs;
}

void ShaderManager::AddPixelShader(ShaderKeys key, std::string path, std::shared_ptr<IPixelShader> ps, 
									ComPtr<ID3D11Device1> dev, int constBufSize) {
	Array<byte>^ psFile = LoadShaderFile(path);
	ComPtr<ID3D11PixelShader> pixelShader = {};
	dev->CreatePixelShader(psFile->Data, psFile->Length, nullptr, pixelShader.GetAddressOf());
	
	ps->m_directXObjPS = pixelShader;
	ps->m_shaderFile = psFile;
	ps->m_constBufferSize = constBufSize;
	ps->m_key = key;
	
	m_pixelShaders[key] = ps;
}

void ShaderManager::AddEntityToShaders(ShaderKeys shader, Entity* entity) {
	m_shadersMap[shader].push_back(entity);
}

std::shared_ptr<IVertexShader> ShaderManager::GetVertexShader(ShaderKeys key) {
	return m_vertexShaders[key]; 
};

std::shared_ptr<IPixelShader> ShaderManager::GetPixelShader(ShaderKeys key) {
	return m_pixelShaders[key]; 
};

const void* ShaderManager::GetShinyMatVSConstBufferVals(XMMATRIX matFinal, XMMATRIX rot, XMFLOAT3 camPos, 
														XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projMatrix) {
	SHINYMATCONSTBUFF_VS* constBufVals = new SHINYMATCONSTBUFF_VS();
	constBufVals->matFinal = matFinal;
	constBufVals->rotation = rot;
	constBufVals->color = XMVectorSet(1.0f, 0.4f, 0.4f, 1.0f);
	constBufVals->camPos = camPos;

	constBufVals->worldMatrix = worldMatrix;
	constBufVals->viewMatrix = viewMatrix;
	constBufVals->projectionMatrix = projMatrix;

	return constBufVals;
}

// todo
const void * ShaderManager::GetShinyMatPSConstBufferVals(XMMATRIX matFinal, XMMATRIX rot, XMFLOAT3 camPos, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projMatrix)
{
	return nullptr;
}

const void* ShaderManager::GetTextureVSConstBufferVals(XMMATRIX matFinal, XMMATRIX rot) {
	CONSTANTBUFFER_VS* constBufVals = new CONSTANTBUFFER_VS();

	constBufVals->matFinal = matFinal;
	constBufVals->rotation = rot;

	// todo set directional light to be a global set of values, accessible from any shader (then do the same for point lights?)
	constBufVals->ambientColor = XMVectorSet(0.4f, 0.4f, 0.4f, 1.0f); // the higher the RGB values, the brighter the light
	constBufVals->diffuseColor = XMVectorSet(0.5f, 0.5f, 0.5f, 1.0f);
	constBufVals->diffuseVector = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);

	return constBufVals;
}