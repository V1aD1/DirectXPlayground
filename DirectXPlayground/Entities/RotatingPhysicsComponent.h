#pragma once

#include "PhysicsComponent.h"

class RotatingPhysicsComponent: public PhysicsComponent
{
public:
	RotatingPhysicsComponent();

	void Update(Entity& self, float dt) override;

	~RotatingPhysicsComponent();
};

