#pragma once
#include "../GraphicsComponent.h"

class FlatMeshGraphicsComponent : public GraphicsComponent
{
public:
	FlatMeshGraphicsComponent(int verticesPerRow = 1, int verticesPerCol = 1);

	~FlatMeshGraphicsComponent();
};

