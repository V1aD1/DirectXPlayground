#include "pch.h"
#include "CameraPhysicsComponent.h"

CameraPhysicsComponent::CameraPhysicsComponent()
{
	m_maxSpeed = 10.0f;
	m_accRate = 6.0f;
	m_dragRate = m_accRate * 3.0;
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

void CameraPhysicsComponent::Accelerate() {
	m_isAccelerating = true;
	PhysicsComponent::Accelerate();
}

void CameraPhysicsComponent::Decelerate() {
	m_isAccelerating = true;
	PhysicsComponent::Decelerate();
}

void CameraPhysicsComponent::AccelerateInDir(Vector3 dir)
{
	m_isAccelerating = true;
	PhysicsComponent::AccelerateInDir(dir);
}

void CameraPhysicsComponent::Update(Entity& self, float dt)
{
	m_velocity += m_acceleration * dt;
	
	if (m_velocity.Length() > m_maxSpeed) {
		m_velocity.Normalize();
		m_velocity = m_velocity * m_maxSpeed;
	}

	// only apply drag if user isn't directly moving camera right now
	if (!m_isAccelerating) { ApplyDrag(dt); }
	
	m_position += m_velocity * dt;
	UpdateTranslation();
	
	// todo move to ResetFlags()?
	m_isAccelerating = false;
}

CameraPhysicsComponent::~CameraPhysicsComponent()
{
}
