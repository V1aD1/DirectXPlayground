#include "pch.h"
#include "FlatMeshGraphicsComponent.h"
#include "ConstantBuffers.h"


FlatMeshGraphicsComponent::FlatMeshGraphicsComponent(int verticesPerRow, int verticesPerCol)
{
	auto squaresPerRow = 10;
	auto squaresPerCol = 10;

	// todo create mesh with as many vertices as specified
	auto minX = -1.0f , minY = -1.0f;
	auto maxX = 1.0f , maxY = 1.0f;
	auto deltaX = (maxX - minX)/ squaresPerRow;
	auto deltaY = (maxY - minY)/ squaresPerCol;

	for (int i = 0; i < squaresPerRow; i++) {
		m_vertices.push_back({ i*deltaX + minX, minY, 0.0f,         0.0f, 0.0f, 1.0f,    0.0f, 0.0f });
		m_vertices.push_back({ i*deltaX + minX, maxY, 0.0f,         0.0f, 0.0f, 1.0f,    0.0f, 1.0f });
		m_vertices.push_back({ (i + 1)*deltaX + minX, maxY, 0.0f,   0.0f, 0.0f, 1.0f,    1.0f, 1.0f });
		m_vertices.push_back({ (i + 1)*deltaX + minX, minY, 0.0f,   0.0f, 0.0f, 1.0f,    1.0f, 0.0f });
	}

	/*for (int i = 0; i < verticesPerRow-1; i++) {
		m_vertices.push_back({ i*deltaX + minX, minY, 0.0f,         0.0f, 0.0f, 1.0f,    0.0f, 0.0f });
		m_vertices.push_back({ i*deltaX + minX, maxY, 0.0f,         0.0f, 0.0f, 1.0f,    0.0f, 1.0f });
		m_vertices.push_back({ (i + 1)*deltaX + minX, maxY, 0.0f,   0.0f, 0.0f, 1.0f,    1.0f, 1.0f });
		m_vertices.push_back({ (i + 1)*deltaX + minX, minY, 0.0f,   0.0f, 0.0f, 1.0f,    1.0f, 0.0f });
	}*/

	for (int i = 0; i < squaresPerRow; i++) {
		m_indices.push_back(i);
		m_indices.push_back(i+1);
		m_indices.push_back(i+2);
		m_indices.push_back(i+2);
		m_indices.push_back(i+3);
		m_indices.push_back(i);
	}

	/*m_vertices = {
		{-1.0f, -1.0f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f},
		{-1.0f, 1.0f, 0.0f,		0.0f, 0.0f, 1.0f,   0.0f, 1.0f},
		{1.0f, 1.0f, 0.0f,		0.0f, 0.0f, 1.0f,   1.0f, 1.0f},
		{1.0f, -1.0f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 0.0f}
	};

	// using clockwise order
	m_indices = {
		0, 1, 2,
		2, 3, 0
	};*/

	SetupBuffers();
}


FlatMeshGraphicsComponent::~FlatMeshGraphicsComponent()
{
}
