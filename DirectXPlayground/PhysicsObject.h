#pragma once
#define NOMINMAX

#include <algorithm>
#include <vector>
#include <SimpleMath.h>

#include "Globals.h"

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace DirectX;
using namespace Platform;
using namespace Windows::System;
using namespace SimpleMath;

class PhysicsObject {
protected:
	XMMATRIX m_translation;
	Vector3 m_velocity;

public:
	Vector3 m_position;

protected:
	void Update(float dt) {
		Vector3 dVel = m_velocity * dt;
		m_position += dVel;
		m_translation = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	}
	void SetVelocity(Vector3 vel) { m_velocity = vel; }

};

class Camera : public PhysicsObject {
private:
	Vector3 m_forward = { 0,0,1 };
	float m_acc = 1.0f;
	float m_maxAcc = 100.0f;
	float m_currAcc = 0.0f;
	float m_drag = m_acc / 2;

public:
	void Accelerate(float dt) { 
		ApplyAcceleration(m_acc * dt);
	}

	void Decelerate(float dt) {
		ApplyAcceleration(-1 * m_acc * dt);
	}

	void Update(float dt) {
		ApplyDrag(dt);

		float dSpeed = m_currAcc * dt;
		Vector3 dVel = m_forward * dSpeed;
		m_velocity += dVel;
		PhysicsObject::Update(dt);
	}

private:
	void ApplyDrag(float dt) {
		if (m_currAcc > 0) {
			if (m_currAcc > m_drag * dt){ m_currAcc -= m_drag * dt; }
			else { m_currAcc = 0; }
		}
		else if (m_currAcc < 0) {
			if (m_currAcc < m_drag * dt) { m_currAcc += m_drag * dt; }
			else { m_currAcc = 0; }
		}
	}

	void ApplyAcceleration(float dAcc) {
		m_currAcc += dAcc;

		// todo please tell there's a better way to use std::min than this! https://stackoverflow.com/questions/13416418/define-nominmax-using-stdmin-max/13420838
		m_currAcc = (std::min)(m_currAcc, m_maxAcc);
	}
};