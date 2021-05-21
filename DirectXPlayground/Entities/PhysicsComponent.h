#pragma once

#include <SimpleMath.h>

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace DirectX;
using namespace Platform;
using namespace Windows::System;
using namespace SimpleMath;

class Entity;

class PhysicsComponent
{
protected:
	Vector3 m_velocity{};
	Vector3 m_acceleration{};

	float m_accRate{ 0 };

	bool m_isAccelerating{ false };

public:
	Vector3 m_position{};
	Vector3 m_rotation{};
	Vector3 m_forward{ Vector3::Forward };

public:
	PhysicsComponent();
	PhysicsComponent(Vector3 pos, Vector3 rot) : m_position(pos), m_rotation(rot) {};

	void SetVelocity(Vector3 vel);
	void SetAcceleration(Vector3 acc);
	virtual void Accelerate();
	virtual void Decelerate();
	virtual void Update(Entity& self, float dt);

	~PhysicsComponent();
};

