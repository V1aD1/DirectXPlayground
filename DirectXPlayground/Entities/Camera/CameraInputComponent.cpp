#include "pch.h"
#include "CameraInputComponent.h"
#include "../InputHandler.h"
#include "../Entity.h"
#include "../PhysicsComponent.h"
#include "Logger.h"

#include <SimpleMath.h>

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

	// mouse look-around logic
	if (!inputHandler.rmbFlag) { 
		m_isMouseLooking = false;
		return; 
	}

	// want first click to not automatically make camera look in random direction
	// so first click is more of a setup step, next frame we actually start looking
	if (!m_isMouseLooking) {
		m_isMouseLooking = true;
		m_currPos = inputHandler.currentPointerPos;
	}
	if (inputHandler.currentPointerPos != m_currPos) {
		auto newPos = inputHandler.currentPointerPos;
		auto deltaX = newPos.X - m_currPos.X;
		auto deltaY = -1* (newPos.Y - m_currPos.Y);
		Vector3 rot = physics->GetRotation();
		Vector3 newRot = rot + Vector3{ deltaX * m_rotSpeed, deltaY * m_rotSpeed, 0 };

		physics->SetRotation(newRot);
		auto lookDir = physics->GetForwardDir();
		Logger::Log("New camera rotation: " + std::to_string(newRot.x) + ", " + std::to_string(newRot.y));
		Logger::Log("New camera look dir: " + std::to_string(lookDir.x) + ", " + std::to_string(lookDir.y));

		m_currPos = newPos;
	}
}

CameraInputComponent::~CameraInputComponent()
{
}
