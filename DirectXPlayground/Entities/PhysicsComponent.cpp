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

void PhysicsComponent::Accelerate()
{
	m_acceleration = m_forward * m_accRate;
}

void PhysicsComponent::Decelerate()
{
	m_acceleration = -1* m_forward * m_accRate;
}

PhysicsComponent::~PhysicsComponent()
{
}
