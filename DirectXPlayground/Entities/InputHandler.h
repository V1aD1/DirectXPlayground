#pragma once

using namespace Windows::System;
using namespace Windows::UI::Input;

class InputHandler
{
public:
	InputHandler();

	// todo make these into private function friended to App.cpp?
	void KeyDown(VirtualKey key);
	void KeyUp(VirtualKey key);
	void OnMouseMoved(PointerPoint^ latestPointerInfo);
	void OnMouseKeyChanged(PointerPoint^ latestPointerInfo);

	bool upFlag{ false };
	bool downFlag{ false };
	bool leftFlag{ false };
	bool rightFlag{ false };
	bool qKeyFlag{ false };
	bool eKeyFlag{ false };

	bool lmbFlag{ false };
	bool rmbFlag{ false };

	Windows::Foundation::Point currentPointerPos{};

	~InputHandler();
};

