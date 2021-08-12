#pragma once
#include "../GraphicsComponent.h"

class CylinderGraphicsComponent: public GraphicsComponent
{
public:
	CylinderGraphicsComponent(float radius=1.0f, float height=1.0f, int numStacks=1, int numSlices=10);
	~CylinderGraphicsComponent();
};

