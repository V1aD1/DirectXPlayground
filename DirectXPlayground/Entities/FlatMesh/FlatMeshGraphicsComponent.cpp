#include "pch.h"
#include "FlatMeshGraphicsComponent.h"
#include "ConstantBuffers.h"


FlatMeshGraphicsComponent::FlatMeshGraphicsComponent(int squaresPerRow, int squaresPerCol)
{
	auto minX = -1.0f , minY = -1.0f;
	auto maxX = 1.0f , maxY = 1.0f;
	auto deltaX = (maxX - minX)/ squaresPerRow;
	auto deltaY = (maxY - minY)/ squaresPerCol;

	for (int i = 0; i < squaresPerRow; i++) {
		auto left = i * deltaX + minX;
		auto right = (i + 1)*deltaX + minX;
		for (int j = 0; j < squaresPerCol; j++) {
			auto down = j * deltaY + minY;
			auto up = (j + 1)*deltaY + minY;
			
			// clock wise order
			m_vertices.push_back({ left, j * deltaY + minY, 0.0f,        0.0f, 0.0f, 1.0f,    0.0f, 0.0f });
			m_vertices.push_back({ left, (j+1)*deltaY + minY, 0.0f,      0.0f, 0.0f, 1.0f,    0.0f, 1.0f });
			m_vertices.push_back({ right, (j + 1)*deltaY + minY, 0.0f,   0.0f, 0.0f, 1.0f,    1.0f, 1.0f });
			m_vertices.push_back({ right, j*deltaY + minY, 0.0f,         0.0f, 0.0f, 1.0f,    1.0f, 0.0f });
		}
	}

	// clockwise order
	for (int i = 0; i < m_vertices.size(); i+=4) {
		m_indices.push_back(i);
		m_indices.push_back(i + 1);
		m_indices.push_back(i + 2);
		m_indices.push_back(i + 2);
		m_indices.push_back(i + 3);
		m_indices.push_back(i);
	}

	SetupBuffers();
}


FlatMeshGraphicsComponent::~FlatMeshGraphicsComponent()
{
}
