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
class IPixelShader;

class ShaderManager {
private:
	std::map<ShaderKeys, std::shared_ptr<IVertexShader>> m_vertexShaders{};
	std::map<ShaderKeys, std::shared_ptr<IPixelShader>> m_pixelShaders{};

public:
	std::map<ShaderKeys, std::vector<Entity*>> m_shadersMap{};

private:
	// todo should use and split up into SetupVS and AddVS or something...
	void SetupAndAddVertexShader(ShaderKeys key, std::string path, std::shared_ptr<IVertexShader> vs, ComPtr<ID3D11Device1> dev, int constBufSize);
	void AddPixelShader(ShaderKeys key, std::string path, std::shared_ptr<IPixelShader> ps, ComPtr<ID3D11Device1> dev, int constBufSize);

public:
	ShaderManager(ComPtr<ID3D11Device1> dev);
	~ShaderManager();
	void AddEntityToShaders(ShaderKeys shader, Entity* entity);
	std::shared_ptr<IVertexShader> GetVertexShader(ShaderKeys key);
	std::shared_ptr<IPixelShader> GetPixelShader(ShaderKeys key);
	
	// const buffer constructor functions for different vertex shaders
	static const void* GetShinyMatVSConstBufferVals(XMMATRIX matFinal, XMMATRIX rot, XMFLOAT3 camPos, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projMatrix);
	static const void* GetShinyMatPSConstBufferVals(XMMATRIX matFinal, XMMATRIX rot, XMFLOAT3 camPos, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projMatrix);
	
	static const void* GetTextureVSConstBufferVals(XMMATRIX matFinal, XMMATRIX rot);
};