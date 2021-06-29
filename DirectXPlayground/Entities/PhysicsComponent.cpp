#include "pch.h"
#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent()
{
}

PhysicsComponent::PhysicsComponent(Vector3 pos, Vector3 rotInRad, XMMATRIX scale) :
	m_position(pos), m_scale(scale)
{
	// this operation also sets thhe forward direction,
	// which is why rotation isn't setup using initializer list
	SetRotation(rotInRad);
}

void PhysicsComponent::Update(Entity& self, float dt)
{
	m_velocity += m_acceleration * dt;
	m_position += m_velocity * dt;
	UpdateTranslation();
	
	auto quaternion = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&(m_rotationRad)));
	m_rotQuaternion = XMMatrixRotationQuaternion(quaternion);
}

void PhysicsComponent::UpdateForwardDir()
{
	float y = -1 * sinf(m_rotationRad.y);
	float r = cosf(m_rotationRad.y);
	float z = r * cosf(m_rotationRad.x);
	float x = r * sinf(m_rotationRad.x);
	m_forward = Vector3(x, y, z);
}

void PhysicsComponent::UpdateTranslation()
{
	m_translation = XMMatrixTranslationFromVector(XMLoadFloat3(&(m_position)));
}


void PhysicsComponent::AccelerateInDir(Vector3 dir)
{
	m_acceleration = dir * m_accRate;
}

Vector3 PhysicsComponent::GetRotation()
{
	return m_rotationRad;
}

void PhysicsComponent::SetRotation(Vector3 newRot)
{
	m_rotationRad = newRot;
	UpdateForwardDir();
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

const Vector3& PhysicsComponent::GetVelocity()
{
	return m_velocity;
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

Vector3 PhysicsComponent::GetForwardDir()
{
	return m_forward;
}

PhysicsComponent::~PhysicsComponent()
{
}
