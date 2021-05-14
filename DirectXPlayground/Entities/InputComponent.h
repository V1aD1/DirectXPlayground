#pragma once

class Entity;
class InputHandler;

class InputComponent
{
public:
	InputComponent();

	void Update(Entity& self, float dt, const InputHandler& inputHandler);

	~InputComponent();
};

