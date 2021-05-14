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
	bool isAccelerating = false;
	if (inputHandler.upFlag) { 
		isAccelerating = true;
		physics->Accelerate(dt); 
	}
	if (inputHandler.downFlag) { 
		isAccelerating = true;
		physics->Decelerate(dt); 
	}
	physics->SetIsAcceleratingFlag(isAccelerating);
}


CameraInputComponent::~CameraInputComponent()
{
}
