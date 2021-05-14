#pragma once
#define NOMINMAX

#include <SimpleMath.h>

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace DirectX;
using namespace Platform;
using namespace Windows::System;
using namespace SimpleMath;

class PhysicsObject {
protected:
	XMMATRIX m_translation;
	Vector3 m_velocity;

public:
	Vector3 m_position;

protected:
	void Update(float dt) {
		Vector3 dVel = m_velocity * dt;
		m_position += dVel;
		m_translation = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	}
	void SetVelocity(Vector3 vel) { m_velocity = vel; }

};

class Camera : public PhysicsObject {
private:
	Vector3 m_forward = Vector3::Forward;

	float m_accRate = 4.0f;
	float m_dragRate = m_accRate * 1.5;

	float m_maxSpeed = 10.0f;

public:

	// todo for now these only handle accelerating forward
	void Accelerate(float dt) { 
		ApplyAcceleration(dt, m_forward);
	}

	// todo for now these only handle decelerating forward
	void Decelerate(float dt) {
		ApplyAcceleration(dt, -1 * m_forward);
	}

	void Update(float dt) {
		ApplyDrag(dt);
		PhysicsObject::Update(dt);
	}

private:
	void ApplyDrag(float dt) {

		// find direction we're moving in
		auto velDir = Vector3(m_velocity);
		velDir.Normalize();
		auto dragVel = velDir * -1.0f * m_dragRate * dt;

		// decelerate in opposite direction
		if (m_velocity.Length() > dragVel.Length()) {
			m_velocity += dragVel;
		}

		// we don't want our drag to reverse us, so set to 0
		else { m_velocity *= 0; }
	}

	void ApplyAcceleration(float dt, Vector3 dir) {
		auto dVel = dir * dt * m_accRate;
		m_velocity += dVel;

		if (m_velocity.Length() > m_maxSpeed) {
			m_velocity.Normalize();
			m_velocity = m_velocity * m_maxSpeed;
		}
	}
};