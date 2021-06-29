#pragma once
#include "../PhysicsComponent.h"

class CameraPhysicsComponent : public PhysicsComponent
{
private:
	float m_maxSpeed{ 10.0f };
	float m_dragRate{0};

private:
	void ApplyDrag(float dt);

public:
	CameraPhysicsComponent(Vector3 pos, Vector3 rotRad);

	void Accelerate() override;
	void Decelerate() override;
	void Update(Entity& self, float dt) override;
	void AccelerateInDir(Vector3 dir) override;

	~CameraPhysicsComponent();
};

