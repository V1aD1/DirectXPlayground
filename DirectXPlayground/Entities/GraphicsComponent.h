#pragma once

#include <vector>

#include "Vertex.h"
#include "Globals.h"
#include "Entities/PhysicsComponent.h"

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace DirectX;
using namespace Platform;
using namespace Windows::System;

class GraphicsComponent
{
public:
	D3D11_BUFFER_DESC m_vbDesc{}; // should be moved out to VertexShader
	D3D11_BUFFER_DESC m_ibDesc{}; // should be moved out to VertexShader
	D3D11_SUBRESOURCE_DATA m_vertexData{};
	D3D11_SUBRESOURCE_DATA m_indexData{};
	std::vector<VERTEX> m_vertices{};
	std::vector<short> m_indices{};
	std::vector<ComPtr<ID3D11ShaderResourceView>> m_textures{};
	D3D_PRIMITIVE_TOPOLOGY m_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

protected:
	void SetupBuffers();

public:
	GraphicsComponent();
	~GraphicsComponent();

	void AddTexture(ComPtr<ID3D11ShaderResourceView> texture);
	virtual void Update(Entity& self, float dt);
	virtual const void* GetConstBufferValues();
};

