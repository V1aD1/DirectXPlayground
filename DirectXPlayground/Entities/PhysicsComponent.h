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
private:
	XMMATRIX m_translation{};
	Vector3 m_rotation{};
	XMMATRIX m_rotQuaternion{};
	
	// todo m_forward should be determined according to rotation? Should have Rotate() then
	Vector3 m_forward{ Vector3::Forward };

protected:
	Vector3 m_velocity{};
	Vector3 m_acceleration{};
	Vector3 m_position{};
	XMMATRIX m_scale = XMMatrixScaling(1, 1, 1);

	float m_accRate{ 0 };

	bool m_isAccelerating{ false };

protected:
	void UpdateTranslation();

public:
	PhysicsComponent();
	PhysicsComponent(Vector3 pos, Vector3 rot, XMMATRIX scale = XMMatrixScaling(1, 1, 1)) : 
		m_position(pos), m_rotation(rot), m_scale(scale) {};

	virtual void Accelerate();
	virtual void Decelerate();
	virtual void Update(Entity& self, float dt);
	
	// Accelerates object in a direction without permanently changing its forward direction
	virtual void AccelerateInDir(Vector3 dir);

	// acceleration
	void SetVelocity(Vector3 vel);
	void SetAcceleration(Vector3 acc);

	//rotation
	Vector3 GetRotation();
	void SetRotation(Vector3 newRot);
	XMMATRIX GetQuaternion();

	//position
	Vector3 GetPosition();
	void SetPosition(Vector3 newPos);

	//translation
	XMMATRIX GetTranslation();

	//scale
	XMMATRIX GetScale();

	//forward
	Vector3 GetForwardDir();
	void SetForwardDir(Vector3 newDir);

	~PhysicsComponent();
};

