#pragma once
#include "../GraphicsComponent.h"

class CubeGraphicsComponent : public GraphicsComponent
{
public:
	CubeGraphicsComponent(VertexShaders vs, PixelShaders ps, float textLim = 1.0f);
	~CubeGraphicsComponent();
};

