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
	auto origForwardDir = physics->GetForwardDir();

	if (inputHandler.upFlag) { 
		physics->Accelerate(); 
	}
	if (inputHandler.downFlag) { 
		physics->Decelerate(); 
	}
	if (inputHandler.leftFlag) {
		auto rotMatrix = Matrix::CreateRotationY(XMConvertToRadians(-90));
		auto leftDir = Vector3::TransformNormal(origForwardDir, rotMatrix);
		physics->SetForwardDir(leftDir);
		physics->Accelerate();
		physics->SetForwardDir(origForwardDir);
	}	
	if (inputHandler.rightFlag) {
		auto rotMatrix = Matrix::CreateRotationY(XMConvertToRadians(90));
		auto rightDir = Vector3::TransformNormal(origForwardDir, rotMatrix);
		physics->SetForwardDir(rightDir);
		physics->Accelerate();
		physics->SetForwardDir(origForwardDir);
	}
}


CameraInputComponent::~CameraInputComponent()
{
}
