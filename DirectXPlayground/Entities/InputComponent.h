#pragma once

class Entity;
class InputHandler;

class InputComponent
{
public:
	InputComponent();

	virtual void Update(Entity& self, float dt, const InputHandler& inputHandler) = 0;

	~InputComponent();
};

