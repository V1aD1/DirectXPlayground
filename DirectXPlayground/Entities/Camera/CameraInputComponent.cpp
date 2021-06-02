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
	if (inputHandler.leftFlag) {
		auto rotMatrix = Matrix::CreateRotationY(XMConvertToRadians(-90));
		auto dir = Vector3::TransformNormal(physics->GetForwardDir(), rotMatrix);
		physics->AccelerateInDir(dir);
	}	
	if (inputHandler.rightFlag) {
		auto rotMatrix = Matrix::CreateRotationY(XMConvertToRadians(90));
		auto dir = Vector3::TransformNormal(physics->GetForwardDir(), rotMatrix);
		physics->AccelerateInDir(dir);
	}
	if (inputHandler.qKeyFlag) {
		auto dir = Vector3::Up;
		physics->AccelerateInDir(dir);
	}
	if (inputHandler.eKeyFlag) {
		auto dir = Vector3::Down;
		physics->AccelerateInDir(dir);
	}
}

CameraInputComponent::~CameraInputComponent()
{
}
