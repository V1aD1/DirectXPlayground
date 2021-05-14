#include "pch.h"
#include "PhysicsComponent.h"


PhysicsComponent::PhysicsComponent()
{
}

void PhysicsComponent::Update(Entity& self, float dt)
{
	m_velocity += m_acceleration * dt;
	m_position += m_velocity * dt;
}

void PhysicsComponent::SetVelocity(Vector3 vel)
{
	m_velocity = vel;
}

void PhysicsComponent::SetAcceleration(Vector3 acc)
{
	m_acceleration = acc;
}

void PhysicsComponent::Accelerate(float dt)
{
	m_acceleration += m_forward * m_accRate * dt;
}

void PhysicsComponent::Decelerate(float dt)
{
	m_acceleration += -1* m_forward * m_accRate * dt;
}

void PhysicsComponent::SetIsAcceleratingFlag(bool isAccelerating)
{
	m_isAccelerating = isAccelerating;
}

PhysicsComponent::~PhysicsComponent()
{
}
