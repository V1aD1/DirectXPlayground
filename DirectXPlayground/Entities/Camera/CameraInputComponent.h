#pragma once
#include "../InputComponent.h"

class CameraInputComponent : public InputComponent
{
public:
	CameraInputComponent();

	void Update(Entity& self, float dt, const InputHandler& inputHandler) override;

	~CameraInputComponent();
};

