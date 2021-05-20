#include "pch.h"
#include "CameraInputComponent.h"
#include "../InputHandler.h"
#include "../Entity.h"
#include "../PhysicsComponent.h"

CameraInputComponent::CameraInputComponent()
{
}

void CameraInputComponent::Update(Entity& self, float dt, const InputHandler& inputHandler)
{
	auto physics = self.m_physics;
	if (inputHandler.upFlag) { 
		physics->Accelerate(); 
	}
	if (inputHandler.downFlag) { 
		physics->Decelerate(); 
	}
}


CameraInputComponent::~CameraInputComponent()
{
}
