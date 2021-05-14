#pragma once
#include "../PhysicsComponent.h"

class CameraPhysicsComponent : public PhysicsComponent
{
private:
	float m_maxSpeed = 10.0f;
	float m_dragRate = m_accRate * 1.5;

private:
	void ApplyAcceleration(float dt, Vector3 dir);
	void ApplyDrag(float dt);

public:
	CameraPhysicsComponent();

	void Accelerate(float dt) override;
	void Decelerate(float dt) override;
	void Update(Entity& self, float dt) override;

	~CameraPhysicsComponent();
};

