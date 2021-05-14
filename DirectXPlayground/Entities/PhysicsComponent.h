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
	Vector3 m_velocity;

public:
	Vector3 m_position;

public:
	PhysicsComponent();

	void Update(Entity& self, float dt);
	void SetVelocity(Vector3 vel);

	~PhysicsComponent();
};

