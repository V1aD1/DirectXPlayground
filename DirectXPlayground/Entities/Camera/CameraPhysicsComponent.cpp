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
	// find direction we're movin in
	auto dragVel = GetVeloctyDir() * -1.0f * m_dragRate * dt;

	// slow down
	if (GetVelocity().Length() > dragVel.Length()) {
		SetVelocity(GetVelocity() + dragVel);
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
	if (m_isAccelerating) {
		SetVelocity(GetVelocity() + GetAcceleration() * dt);
		if (GetVelocity().Length() > m_maxSpeed) {
			SetVelocity(GetVeloctyDir() * m_maxSpeed);
		}
	}

	// only apply drag if user isn't directly accelerating camera right now
	else { 
		ApplyDrag(dt); 
	}
	
	// need to do this otherwise velocity will inscrease again when calling PhysicsComponent::Update()
	SetAcceleration(Vector3{});
	PhysicsComponent::Update(self, dt);
	
	// todo move to ResetFlags()?
	m_isAccelerating = false;
}

CameraPhysicsComponent::~CameraPhysicsComponent()
{
}
