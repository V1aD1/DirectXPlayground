#pragma once

#include <vector>

#include "Vertex.h"
#include "Globals.h"

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace DirectX;
using namespace Platform;
using namespace Windows::System;

class GraphicsObject {
public:
	D3D11_BUFFER_DESC m_vbDesc;
	D3D11_BUFFER_DESC m_ibDesc;
	D3D11_SUBRESOURCE_DATA m_vertexData;
	D3D11_SUBRESOURCE_DATA m_indexData;
	std::vector<VERTEX> m_vertices;
	std::vector<short> m_indices;
	VertexShaders m_vertexShader;
	PixelShaders m_pixelShader;
	std::vector<ComPtr<ID3D11ShaderResourceView>> m_textures;
	D3D_PRIMITIVE_TOPOLOGY m_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// todo store simple data, and set it in Update or Render functions
	XMMATRIX m_rotation;
	XMMATRIX m_scale = XMMatrixScaling(1, 1, 1);
	XMMATRIX m_translation = XMMatrixTranslation(0, 0, 0); // todo add relative translations

public:
	GraphicsObject() = default;
	virtual ~GraphicsObject() {}

	void AddTexture(ComPtr<ID3D11ShaderResourceView> texture) { m_textures.push_back(texture); }
	virtual void Update(float dt) {}

protected:
	void SetupBuffers() {
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
};

// todo move to own file
class Cube : public GraphicsObject {
	// todo get rid of this
	float m_time;
public:
	// todo account for Cube size, texture, shaders (use fluent builder for this?)
	Cube(float textLim = 1.0f) {
		m_vertices = {
			{ -1.0f, -1.0f, 1.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f },    // side 1
			{ 1.0f, -1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.0f, textLim },
			{ -1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   textLim, 0.0f },
			{ 1.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f,   textLim, textLim },

			{ -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f,  0.0f, 0.0f },    // side 2
			{ -1.0f, 1.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f, textLim },
			{ 1.0f, -1.0f, -1.0f,  0.0f, 0.0f, -1.0f,  textLim, 0.0f },
			{ 1.0f, 1.0f, -1.0f,   0.0f, 0.0f, -1.0f,  textLim, textLim },

			{ -1.0f, 1.0f, -1.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f },    // side 3
			{ -1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 0.0f,   0.0f, textLim },
			{ 1.0f, 1.0f, -1.0f,   0.0f, 1.0f, 0.0f,   textLim, 0.0f },
			{ 1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f,   textLim, textLim },

			{ -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f,  0.0f, 0.0f },    // side 4
			{ 1.0f, -1.0f, -1.0f,  0.0f, -1.0f, 0.0f,  0.0f, textLim },
			{ -1.0f, -1.0f, 1.0f,  0.0f, -1.0f, 0.0f,  textLim, 0.0f },
			{ 1.0f, -1.0f, 1.0f,   0.0f, -1.0f, 0.0f,  textLim, textLim },

			{ 1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f },    // side 5
			{ 1.0f, 1.0f, -1.0f,   1.0f, 0.0f, 0.0f,   0.0f, textLim },
			{ 1.0f, -1.0f, 1.0f,   1.0f, 0.0f, 0.0f,   textLim, 0.0f },
			{ 1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 0.0f,   textLim, textLim },

			{ -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f },    // side 6
			{ -1.0f, -1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,  0.0f, textLim },
			{ -1.0f, 1.0f, -1.0f,  -1.0f, 0.0f, 0.0f,  textLim, 0.0f },
			{ -1.0f, 1.0f, 1.0f,   -1.0f, 0.0f, 0.0f,  textLim, textLim },
		};

		m_indices = {
			0, 1, 2,    // side 1
			2, 1, 3,
			4, 5, 6,    // side 2
			6, 5, 7,
			8, 9, 10,    // side 3
			10, 9, 11,
			12, 13, 14,    // side 4
			14, 13, 15,
			16, 17, 18,    // side 5
			18, 17, 19,
			20, 21, 22,    // side 6
			22, 21, 23,
		};

		// todo maybe factory should call this method...
		SetupBuffers();

		m_vertexShader = VertexShaders::VertexShader1;
		m_pixelShader = PixelShaders::PixelShader1;

		m_scale = XMMatrixScaling(4, 2, 4);
	}

	void Update(float dt) {
		m_time += dt;
		m_rotation = XMMatrixRotationY(XMConvertToRadians(m_time * 20));
	}
};