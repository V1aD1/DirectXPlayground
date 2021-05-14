#include "pch.h"
#include "InputHandler.h"


InputHandler::InputHandler()
{
}

void InputHandler::KeyDown(VirtualKey key)
{
	if (key == VirtualKey::Up) {
		upFlag = true;
	}

	else if (key == VirtualKey::Down) {
		downFlag = true;
	}

	else if (key == VirtualKey::Left) {
		leftFlag = true;
	}

	else if (key == VirtualKey::Right) {
		rightFlag = true;
	}
}

void InputHandler::KeyUp(VirtualKey key)
{
	if (key == VirtualKey::Up) {
		upFlag = false;
	}

	else if (key == VirtualKey::Down) {
		downFlag = false;
	}

	else if (key == VirtualKey::Left) {
		leftFlag = false;
	}

	else if (key == VirtualKey::Right) {
		rightFlag = false;
	}
}


InputHandler::~InputHandler()
{
}
