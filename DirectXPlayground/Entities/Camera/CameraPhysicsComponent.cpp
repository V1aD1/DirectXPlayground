#include "pch.h"
#include "CameraPhysicsComponent.h"

CameraPhysicsComponent::CameraPhysicsComponent()
{
	m_accRate = 4.0f;
}

void CameraPhysicsComponent::ApplyAcceleration(float dt, Vector3 dir)
{
	auto dVel = dir * dt * m_accRate;
	m_velocity += dVel;

	if (m_velocity.Length() > m_maxSpeed) {
		m_velocity.Normalize();
		m_velocity = m_velocity * m_maxSpeed;
	}
}

void CameraPhysicsComponent::ApplyDrag(float dt)
{
	// find direction we're moving in
	auto velDir = Vector3(m_velocity);
	velDir.Normalize();
	auto dragVel = velDir * -1.0f * m_dragRate * dt;

	// slow down
	if (m_velocity.Length() > dragVel.Length()) {
		m_velocity += dragVel;
	}

	// we don't want our drag to reverse us, so set to 0
	else { m_velocity *= 0; }
}

void CameraPhysicsComponent::Accelerate(float dt) {
	m_isAccelerating = true;
	PhysicsComponent::Accelerate(dt);
	//ApplyAcceleration(dt, m_forward);
}

void CameraPhysicsComponent::Decelerate(float dt) {
	m_isAccelerating = true;
	PhysicsComponent::Decelerate(dt);
	//ApplyAcceleration(dt, -1 * m_forward);
}

void CameraPhysicsComponent::Update(Entity& self, float dt)
{
	m_velocity += m_acceleration * dt;

	// only apply drag if user isn't directly moving camera right now
	if (!m_isAccelerating){ ApplyDrag(dt); }
	
	if (m_velocity.Length() > m_maxSpeed) {
		m_velocity.Normalize();
		m_velocity = m_velocity * m_maxSpeed;
	}
	
	m_position += m_velocity * dt;
	
	// todo move to ResetFlags()?
	m_isAccelerating = false;
}

CameraPhysicsComponent::~CameraPhysicsComponent()
{
}
