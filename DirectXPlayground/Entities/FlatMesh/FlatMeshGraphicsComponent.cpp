#include "pch.h"
#include "FlatMeshGraphicsComponent.h"
#include "ConstantBuffers.h"


FlatMeshGraphicsComponent::FlatMeshGraphicsComponent(int verticesPerRow, int verticesPerCol)
{
	m_vertices = {
		{-1.0f, -1.0f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f},
		{-1.0f, 1.0f, 0.0f,		0.0f, 0.0f, 1.0f,   0.0f, 1.0f},
		{1.0f, 1.0f, 0.0f,		0.0f, 0.0f, 1.0f,   1.0f, 1.0f},
		{1.0f, -1.0f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 0.0f}
	};

	// using clockwise order
	m_indices = {
		0, 1, 2,
		2, 3, 0
	};

	SetupBuffers();
}


FlatMeshGraphicsComponent::~FlatMeshGraphicsComponent()
{
}
