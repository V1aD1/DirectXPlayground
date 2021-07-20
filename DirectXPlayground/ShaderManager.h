#pragma once

#include <string>
#include <vector>
#include <map>

#include "Globals.h"

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace DirectX;
using namespace Platform;
using namespace Windows::System;

// this function loads a file into an Array^
Array<byte>^ LoadShaderFile(std::string file);
class Entity;
class IVertexShader;

class ShaderManager {
private:
	std::map<Shaders, IVertexShader*> m_vertexShaders{};
	std::map<Shaders, ComPtr<ID3D11PixelShader>> m_pixelShaders{};

public:
	std::map<Shaders, std::vector<Entity*>> m_shadersMap{};

private:
	// todo should use and split up into SetupVS and AddVS or something...
	void SetupAndAddVertexShader(Shaders key, std::string path, ComPtr<ID3D11Device1> dev, IVertexShader* vs, int constBufSize);
	void AddVertexShader(Shaders key, std::string path, ComPtr<ID3D11Device1> dev, int constBufSize);
	void AddPixelShader(Shaders key, std::string path, ComPtr<ID3D11Device1> dev);

public:
	ShaderManager(ComPtr<ID3D11Device1> dev);
	void AddEntityToShaders(Shaders shader, Entity* entity);
	IVertexShader* GetVertexShader(Shaders key);
	ComPtr<ID3D11PixelShader> GetPixelShader(Shaders key);
	
	// const buffer constructor functions for different vertex shaders
	static const void* GetShinyMatVSConstBufferVals(XMMATRIX matFinal, XMMATRIX rot, XMFLOAT3 camPos);
	static const void* GetTextureVSConstBufferVals(XMMATRIX matFinal, XMMATRIX rot);
};