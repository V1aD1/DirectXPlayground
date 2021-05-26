#pragma once
class InputComponent;
class PhysicsComponent;
class GraphicsComponent;
class InputHandler;

class Entity
{
public:
	InputComponent * m_input;
	PhysicsComponent* m_physics;
	GraphicsComponent* m_graphics;

public:
	Entity();
	Entity(InputComponent* input, PhysicsComponent* physics, GraphicsComponent* graphics);
	
	void Update(float dt, const InputHandler& eventHandler);

	~Entity();
};

