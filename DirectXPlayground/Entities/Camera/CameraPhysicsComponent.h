#pragma once
#include "../PhysicsComponent.h"

class CameraPhysicsComponent : public PhysicsComponent
{
private:
	float m_maxSpeed = 10.0f;
	float m_dragRate;

private:
	void ApplyDrag(float dt);

public:
	CameraPhysicsComponent();

	void Accelerate() override;
	void Decelerate() override;
	void Update(Entity& self, float dt) override;

	~CameraPhysicsComponent();
};

