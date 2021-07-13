#pragma once
#include "../GraphicsComponent.h"

class CubeGraphicsComponent : public GraphicsComponent
{
public:
	CubeGraphicsComponent(float textLim = 1.0f);

	const void* GetConstBufferValues() override;

	~CubeGraphicsComponent();
};

