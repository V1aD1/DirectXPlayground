#include "pch.h"
#include "RotatingPhysicsComponent.h"


RotatingPhysicsComponent::RotatingPhysicsComponent()
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
