#include "pch.h"
#include "CylinderGraphicsComponent.h"


CylinderGraphicsComponent::CylinderGraphicsComponent(float radius, float height, int numStacks, int numSlices)
{
	// cylinder grows in height from its starting position

	// first, create a diagonal box
	float halfRadius = radius / 2.0f; 

	// use numSlices to determine the bottom points around the radius to build the cylinder
	float sliceAngle = 360.0f / numSlices;

	for (int i = 0; i < numSlices; ++i) {
		float currAngle = i + sliceAngle;
		// hypotenuse = halfRadius
		// using sine, compute length of opposite = equals our x value = sin(currAngle) * hypotenuse
		// using cosine, compute length of opposite = equals our z value = cos(currAngle) * hypotenuse
	}

	// in order to compute normal, simply take vector from center of cylinder (0,0,0) to middle of current face
	// therefore normal is just bottom right corner - bottom left corner!!
	auto bottomLeft = Vector3{ 0.0f, 0.0f, -halfRadius };
	auto bottomRight = Vector3{ halfRadius, 0.0f, 0.0f };

	auto normal = bottomRight - bottomLeft;

	m_vertices.push_back({ 0.0f, 0.0f, -halfRadius,        normal.x, normal.y, normal.z,   0.0f, 0.0f });
	m_vertices.push_back({ 0.0f, height, -halfRadius,      normal.x, normal.y, normal.z,   0.0f, 1.0f });
	m_vertices.push_back({ halfRadius, height, 0.0f,       normal.x, normal.y, normal.z,   1.0f, 1.0f });
	m_vertices.push_back({ halfRadius, 0.0f, 0.0f,         normal.x, normal.y, normal.z,   1.0f, 0.0f });

	m_indices.push_back(0);
	m_indices.push_back(1);
	m_indices.push_back(2);
	m_indices.push_back(2);
	m_indices.push_back(3);
	m_indices.push_back(0);

	// todo add top and bottom covers

	SetupBuffers();
}


CylinderGraphicsComponent::~CylinderGraphicsComponent()
{
}
