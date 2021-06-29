#include "pch.h"
#include "RotatingPhysicsComponent.h"


RotatingPhysicsComponent::RotatingPhysicsComponent(Vector3 pos, Vector3 rotInRad, XMMATRIX scale) : 
	PhysicsComponent(pos, rotInRad, scale)
{
}

void RotatingPhysicsComponent::Update(Entity& self, float dt) {
	auto currRot = GetRotation();
	currRot.y += dt;
	SetRotation(currRot);
	PhysicsComponent::Update(self, dt);
}

RotatingPhysicsComponent::~RotatingPhysicsComponent()
{
}
