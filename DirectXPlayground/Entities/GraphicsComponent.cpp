#include "pch.h"
#include "GraphicsComponent.h"


GraphicsComponent::GraphicsComponent()
{
}

void GraphicsComponent::AddTexture(ComPtr<ID3D11ShaderResourceView> texture) {
	m_textures.push_back(texture);
}

void GraphicsComponent::Update(Entity& self, float dt) {

}

const void * GraphicsComponent::GetConstBufferValues()
{
	return nullptr;
}

void GraphicsComponent::SetupBuffers() {
	// struct specifying properties of the buffer
	m_vbDesc = { 0 };

	// size of the buffer that we'll create, in bytes
	m_vbDesc.ByteWidth = sizeof(VERTEX) * m_vertices.size();

	// what kind of buffer we're making (vertex buffer)
	m_vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// data we're going to store in the vertex buffer
	m_vertexData = { &m_vertices[0], 0, 0 };

	// repeat for index buffer
	m_ibDesc = { 0 };
	m_ibDesc.ByteWidth = sizeof(short) * m_indices.size(); // indices are stored in short values
	m_ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_indexData = { &m_indices[0], 0, 0 };
}

GraphicsComponent::~GraphicsComponent()
{
}
