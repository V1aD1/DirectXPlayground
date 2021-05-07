#pragma once
#define NOMINMAX

#include <algorithm>
#include <vector>

#include "Globals.h"

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace DirectX;
using namespace Platform;
using namespace Windows::System;

class PhysicsObject {
protected:
	XMMATRIX m_translation;
	XMFLOAT3 m_velocity;

public:
	XMFLOAT3 m_position;

protected:
	void Update(float dt) {
		XMFLOAT3 dVel = XMFLOAT3(m_velocity.x * dt, m_velocity.y * dt, m_velocity.z * dt);
		m_position.x += dVel.x;
		m_position.y += dVel.y;
		m_position.z += dVel.z;
		m_translation = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	}
	void SetVelocity(XMFLOAT3 vel) { m_velocity = vel; }

};

class Camera : public PhysicsObject {
private:
	XMFLOAT3 m_forward = XMFLOAT3(0,0,1);
	float m_acc = 1.0f;
	float m_maxAcc = 100.0f;
	float m_currAcc = 0.0f;

public:
	void Accelerate(float dt) { 
		m_currAcc += m_acc * dt;

		// todo please tell there's a better way to use std::min than this! https://stackoverflow.com/questions/13416418/define-nominmax-using-stdmin-max/13420838
		m_currAcc = (std::min)(m_currAcc, m_maxAcc);
	}

	void Update(float dt) {
		float dSpeed = m_currAcc * dt;
		XMFLOAT3 dVelocity = XMFLOAT3(m_forward.x * dSpeed, m_forward.y * dSpeed, m_forward.z * dSpeed);
		
		// TODO overload operators as free function? Or create own math library for this?
		m_velocity.x += dVelocity.x;
		m_velocity.y += dVelocity.y;
		m_velocity.z += dVelocity.z;
		PhysicsObject::Update(dt);
	}
};