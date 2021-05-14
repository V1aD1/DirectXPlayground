#include "pch.h"
#include "Entity.h"

#include "InputComponent.h"
#include "PhysicsComponent.h"


Entity::Entity()
{
}

Entity::Entity(InputComponent *input, PhysicsComponent* physics)
{
	m_input = input;
	m_physics = physics;
}

void Entity::Update(float dt, const InputHandler& inputHandler)
{
	if (m_input) {
		m_input->Update(*this, dt, inputHandler);
	}

	if (m_physics) {
		m_physics->Update(*this, dt);
	}
}


Entity::~Entity()
{
}
