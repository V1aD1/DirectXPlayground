#pragma once

using namespace Windows::System;

class InputHandler
{
public:
	InputHandler();

	// todo make these into private function friended to App.cpp?
	void KeyDown(VirtualKey key);
	void KeyUp(VirtualKey key);

	bool upFlag = false;
	bool downFlag = false;
	bool leftFlag = false;
	bool rightFlag = false;

	~InputHandler();
};
