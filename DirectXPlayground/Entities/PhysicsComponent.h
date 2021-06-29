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
	Vector3 m_rotationRad{};
	XMMATRIX m_rotQuaternion{};
	Vector3 m_forward{ Vector3::Forward };
	Vector3 m_velocity{};
	Vector3 m_acceleration{};
	Vector3 m_position{};
	XMMATRIX m_scale = XMMatrixScaling(1, 1, 1);

private:
	void UpdateForwardDir();
	void UpdateTranslation();

protected:
	// todo make private
	float m_accRate{ 0 };
	bool m_isAccelerating{ false };

public:
	PhysicsComponent();
	PhysicsComponent(Vector3 pos, Vector3 rotInRad, XMMATRIX scale = XMMatrixScaling(1, 1, 1));

	virtual void Accelerate();
	virtual void Decelerate();
	virtual void Update(Entity& self, float dt);
	
	// Accelerates object in a direction without permanently changing its forward direction
	virtual void AccelerateInDir(Vector3 dir);

	// velocity
	Vector3 GetVelocity() const;
	Vector3 GetVeloctyDir() const;
	void SetVelocity(Vector3 vel);

	// acceleration
	Vector3 GetAcceleration() const;
	void SetAcceleration(Vector3 acc);

	//rotation
	Vector3 GetRotation() const ;
	void SetRotation(Vector3 newRot);
	XMMATRIX GetQuaternion() const;

	//position
	Vector3 GetPosition() const;
	void SetPosition(Vector3 newPos);

	//translation
	XMMATRIX GetTranslation() const;

	//scale
	XMMATRIX GetScale() const;

	//forward
	Vector3 GetForwardDir() const;

	~PhysicsComponent();
};

