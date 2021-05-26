#include "pch.h"
#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent()
{
}

void PhysicsComponent::Update(Entity& self, float dt)
{
	m_velocity += m_acceleration * dt;
	m_position += m_velocity * dt;
	UpdateTranslation();
	
	auto quaternion = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&(m_rotation)));
	m_rotQuaternion = XMMatrixRotationQuaternion(quaternion);
}

void PhysicsComponent::UpdateTranslation()
{
	m_translation = XMMatrixTranslationFromVector(XMLoadFloat3(&(m_position)));
}

Vector3 PhysicsComponent::GetRotation()
{
	return m_rotation;
}

void PhysicsComponent::SetRotation(Vector3 newRot)
{
	// todo update m_forward as well
	m_rotation = newRot;
}

XMMATRIX PhysicsComponent::GetQuaternion()
{
	return m_rotQuaternion;
}

Vector3 PhysicsComponent::GetPosition()
{
	return m_position;
}

void PhysicsComponent::SetPosition(Vector3 newPos)
{
	m_position = newPos;
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

XMMATRIX PhysicsComponent::GetTranslation()
{
	return m_translation;
}

XMMATRIX PhysicsComponent::GetScale()
{
	return m_scale;
}

PhysicsComponent::~PhysicsComponent()
{
}
