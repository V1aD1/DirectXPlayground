#include "pch.h"
#include "CameraInputComponent.h"
#include "../InputHandler.h"
#include "../Entity.h"
#include "../PhysicsComponent.h"
#include "Logger.h"

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

	// todo handle mouse delta moving, by changing the direction mouse is looking in,
	// then using XMMatrixLookToLH() in game.cpp
	if (inputHandler.currentPointerPos != m_currPos) {
		auto newPos = inputHandler.currentPointerPos;
		auto deltaX = newPos.X - m_currPos.X;
		auto deltaY = newPos.Y - m_currPos.Y;
		Vector3 rot = physics->GetRotation();
		Vector3 newRot = rot + Vector3{ deltaX * m_rotSpeed, deltaY * m_rotSpeed, 0 };

		physics->SetRotation(newRot);

		Logger::Log("New camera rotation: " + std::to_string(newRot.x) + ", " + std::to_string(newRot.y));

		m_currPos = newPos;
	}
}

CameraInputComponent::~CameraInputComponent()
{
}
