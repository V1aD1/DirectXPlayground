#include "pch.h"
#include "CameraPhysicsComponent.h"

CameraPhysicsComponent::CameraPhysicsComponent(Vector3 pos, Vector3 rotRad) : PhysicsComponent(pos, rotRad)
{
	m_maxSpeed = 10.0f;
	m_accRate = 6.0f;
	m_dragRate = m_accRate * 3.0;
}

void CameraPhysicsComponent::ApplyDrag(float dt)
{
	// find direction we're moving in
	auto vel = GetVelocity();	
	auto velDir = Vector3(vel);
	velDir.Normalize();
	auto dragVel = velDir * -1.0f * m_dragRate * dt;

	// slow down
	if (vel.Length() > dragVel.Length()) {
		SetVelocity(vel + dragVel);
	}

	// if velocity is less than drag, then we will start to reverse
	// so in this case do NOT apply drag and just set our velocity to 0
	else { SetVelocity(Vector3{ 0, 0, 0 }); }
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
	// todo if accelerating
	//	setup velocityy while accounting for max speed, set acc to 0, then call base update
	if (m_isAccelerating) {
		auto vel = GetVelocity();
		SetVelocity(vel + m_acceleration * dt);
		if (vel.Length() > m_maxSpeed) {

			// todo create helper method for velocity direction
			auto velDir = Vector3(vel);
			velDir.Normalize();
			SetVelocity(velDir * m_maxSpeed);
		}
	}

	else { 
		// only apply drag if user isn't directly accelerating camera right now
		ApplyDrag(dt); 
	}
	
	// need to do this otherwise velocity will inscrease again when calling PhysicsComponent::Update()
	SetAcceleration(Vector3{});
	PhysicsComponent::Update(self, dt);

	// todo fixup these physics components!!
	//	m_velocity etc should all be private! Only accessible through getter methods!!
	
	// todo move to ResetFlags()?
	m_isAccelerating = false;
}

CameraPhysicsComponent::~CameraPhysicsComponent()
{
}
