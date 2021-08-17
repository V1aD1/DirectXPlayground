#include "pch.h"
#include "CylinderGraphicsComponent.h"

#include <math.h>

CylinderGraphicsComponent::CylinderGraphicsComponent(float radius, float height, int numStacks, int numSlices)
{
	// cylinder grows in height from its starting position

	// use numSlices to determine the bottom points around the radius to build the cylinder
	float sliceAngle = 360.0f / numSlices;

	for (int i = 0; i < numSlices - 1; ++i) {
		float startAngle = i * sliceAngle;
		float endAngle = (i + 1) * sliceAngle;

		// hypotenuse = radius
		// using sine, compute length of opposite = equals our x value = sin(currAngle) * hypotenuse
		// using cosine, compute length of opposite = equals our z value = cos(currAngle) * hypotenuse

		auto x1 = sin(startAngle) * radius;
		auto z1 = cos(startAngle) * radius;
		auto x2 = sin(endAngle) * radius;
		auto z2 = cos(endAngle) * radius;


		auto bottomLeft = Vector3(x1, 0.0f, z1);
		auto bottomRight = Vector3(x2, 0.0f, z2);

		// in order to compute normal, simply take vector from center of cylinder (0,0,0) to middle of current face
		// therefore normal is just bottom right corner - bottom left corner!!
		auto normal = bottomRight - bottomLeft;

		m_vertices.push_back({ x1, 0.0f, z1,     normal.x, normal.y, normal.z,   0.0f, 0.0f });
		m_vertices.push_back({ x1, height, z1,   normal.x, normal.y, normal.z,   0.0f, 1.0f });
		m_vertices.push_back({ x2, height, z2,   normal.x, normal.y, normal.z,   1.0f, 1.0f });
		m_vertices.push_back({ x2, 0.0f, z2,     normal.x, normal.y, normal.z,   1.0f, 0.0f });
	}

	for (int i = 0; i < m_vertices.size(); i+=4) {
		m_indices.push_back(i);
		m_indices.push_back(i+1);
		m_indices.push_back(i+2);
		m_indices.push_back(i+2);
		m_indices.push_back(i+3);
		m_indices.push_back(i);
	}

	int coversStartIndex = m_vertices.size();

	// todo add top and bottom covers
	for (int i = 0; i < numSlices - 1; ++i) {
		float startAngle = i * sliceAngle;
		float endAngle = (i + 1) * sliceAngle;

		auto x1 = sin(startAngle) * radius;
		auto z1 = cos(startAngle) * radius;
		auto x2 = sin(endAngle) * radius;
		auto z2 = cos(endAngle) * radius;

		// top cover
		// todo u and v coordinates
		m_vertices.push_back({ 0.0f, height, 0.0f,  0.0f, 1.0f, 0.0f,    0.0f, 0.0f });
		m_vertices.push_back({x1, height, z1,       0.0f, 1.0f, 0.0f,    0.0f, 0.0f});
		m_vertices.push_back({x2, height, z2,       0.0f, 1.0f, 0.0f,    0.0f, 0.0f});

		// bottom cover
		// todo u and v coordinates
		m_vertices.push_back({ 0.0f, 0.0f, 0.0f,   0.0f, -1.0f, 0.0f,    0.0f, 0.0f });
		m_vertices.push_back({ x1, 0.0f, z1,       0.0f, -1.0f, 0.0f,    0.0f, 0.0f });
		m_vertices.push_back({ x2, 0.0f, z2,       0.0f, -1.0f, 0.0f,    0.0f, 0.0f });
	}

	for (int i = coversStartIndex; i < m_vertices.size(); i += 6) {
		m_indices.push_back(i);
		m_indices.push_back(i+1);
		m_indices.push_back(i+2);

		m_indices.push_back(i + 3);
		m_indices.push_back(i + 4);
		m_indices.push_back(i + 5);
	}

	SetupBuffers();
}


CylinderGraphicsComponent::~CylinderGraphicsComponent()
{
}
