#pragma once

#include <vector>

#include "Vertex.h"

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace DirectX;
using namespace Platform;
using namespace Windows::System;

class IGraphicsObject {
protected:
	std::vector<VERTEX> m_vertices;
	std::vector<short> m_indices;

public:
	virtual std::vector<VERTEX> GetVertices() { return m_vertices; };
	virtual std::vector<short> GetIndices() { return m_indices; };
};

// todo move to own file
class Cube : public IGraphicsObject {
public:
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
	}
};