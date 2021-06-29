#pragma once

#include "PhysicsComponent.h"

class RotatingPhysicsComponent: public PhysicsComponent
{
public:
	RotatingPhysicsComponent(Vector3 pos, Vector3 rotInRad = {0, 0, 0}, XMMATRIX scale = XMMatrixScaling(1, 1, 1));

	void Update(Entity& self, float dt) override;

	~RotatingPhysicsComponent();
};

