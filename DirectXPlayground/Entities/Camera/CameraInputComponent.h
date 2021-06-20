#pragma once
#include "../InputComponent.h"

using namespace Windows::UI::Input;


class CameraInputComponent : public InputComponent
{
private:
	Windows::Foundation::Point m_currPos{};
	float m_rotSpeed = 0.001f;
	bool m_isMouseLooking = false;

public:
	CameraInputComponent();

	void Update(Entity& self, float dt, const InputHandler& inputHandler) override;

	~CameraInputComponent();
};

