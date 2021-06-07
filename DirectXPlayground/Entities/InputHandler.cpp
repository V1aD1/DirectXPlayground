#include "pch.h"
#include "InputHandler.h"
#include "Logger.h"


InputHandler::InputHandler()
{
}

void InputHandler::KeyDown(VirtualKey key)
{
	switch (key) {
		case VirtualKey::Up:
			upFlag = true;
			break;
		case VirtualKey::Down:
			downFlag = true;
			break;
		case VirtualKey::Left:
			leftFlag = true;
			break;
		case VirtualKey::Right:
			rightFlag = true;
			break;
		case VirtualKey::W:
			upFlag = true;
			break;
		case VirtualKey::S:
			downFlag = true;
			break;
		case VirtualKey::A:
			leftFlag = true;
			break;
		case VirtualKey::D:
			rightFlag = true;
			break;
		case VirtualKey::Q:
			qKeyFlag = true;
			break;
		case VirtualKey::E:
			eKeyFlag = true;
			break;
	}
}

void InputHandler::KeyUp(VirtualKey key)
{
	switch (key) {
		case VirtualKey::Up:
			upFlag = false;
			break;
		case VirtualKey::Down:
			downFlag = false;
			break;
		case VirtualKey::Left:
			leftFlag = false;
			break;
		case VirtualKey::Right:
			rightFlag = false;
			break;
		case VirtualKey::W:
			upFlag = false;
			break;
		case VirtualKey::S:
			downFlag = false;
			break;
		case VirtualKey::A:
			leftFlag = false;
			break;
		case VirtualKey::D:
			rightFlag = false;
			break;
		case VirtualKey::Q:
			qKeyFlag = false;
			break;
		case VirtualKey::E:
			eKeyFlag = false;
			break;
	}
}

void InputHandler::OnMouseMoved(PointerPoint^ latestPointerInfo)
{
	currentPointerPos = latestPointerInfo->Position;
}


InputHandler::~InputHandler()
{
}
