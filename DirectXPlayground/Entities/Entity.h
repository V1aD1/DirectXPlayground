#pragma once
class InputComponent;
class PhysicsComponent;
class InputHandler;

class Entity
{
private:
	InputComponent * m_input;
	PhysicsComponent* m_physics;
	// todo add graphics component

public:
	Entity();
	Entity(InputComponent* input, PhysicsComponent* physics);
	
	void Update(float dt, const InputHandler& eventHandler);

	~Entity();
};

