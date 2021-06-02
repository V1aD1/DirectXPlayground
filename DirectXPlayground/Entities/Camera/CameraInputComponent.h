#pragma once
#include "../InputComponent.h"

#include <SimpleMath.h>

class CameraInputComponent : public InputComponent
{
public:
	CameraInputComponent();

	void Update(Entity& self, float dt, const InputHandler& inputHandler) override;

	~CameraInputComponent();
};

