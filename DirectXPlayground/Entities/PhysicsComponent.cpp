#include "pch.h"
#include "PhysicsComponent.h"


PhysicsComponent::PhysicsComponent()
{
}

void PhysicsComponent::Update(Entity& self, float dt)
{
	Vector3 dVel = m_velocity * dt;
	m_position += dVel;
}

void PhysicsComponent::SetVelocity(Vector3 vel)
{
	m_velocity = vel;
}


PhysicsComponent::~PhysicsComponent()
{
}
