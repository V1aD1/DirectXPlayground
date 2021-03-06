#include "pch.h"

// all includes must be made AFTER including pch.h!!
#include <sstream>
#include <iostream>

#include "Game.h"
#include "Logger.h"

using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Popups;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Platform;

// the class definition for the core "framework" of our app
ref class App sealed: public IFrameworkView
{
	bool m_windowClosed;
	CGame m_game;

public:
	// some functions called by Windows
	virtual void Initialize(CoreApplicationView^ appView) {
		// set the OnActivated function to handle to Acivated "event"
		appView->Activated += ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &App::OnActivated);
		CoreApplication::Suspending += ref new EventHandler<SuspendingEventArgs^>(this, &App::Suspending);
		CoreApplication::Resuming += ref new EventHandler<Object^>(this, &App::Resuming);

		m_windowClosed = false;
		Logger::Log("Initialize()");
	}

	virtual void SetWindow(CoreWindow^ window){
		window->PointerPressed += ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &App::PointerPressed);
		window->PointerReleased += ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &App::PointerReleased);
		window->PointerMoved += ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &App::PointerMoved);
		window->PointerWheelChanged += ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &App::PointerWheelChanged);
		window->KeyDown += ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &App::KeyDown);
		window->KeyUp += ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &App::KeyUp);
		window->Closed += ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &App::Closed);
		window->VisibilityChanged += ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &App::VisibilityChanged);
	}
	virtual void Load(String^ entryPoint) {}

	virtual void Run() {
		m_game.Initialize();
		
		CoreWindow^ Window = CoreWindow::GetForCurrentThread();

		// repeat until window closes
		while (!m_windowClosed) {
			// run processEvents() to dispatch events
			// ProcessAllIfPresent makes ProcessEvents return once all events have been processed
			Window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

			// run the rest of the game code here
			m_game.Update();
			m_game.Render();
		}

		m_game.Finalize();
	}

	virtual void Uninitialize() {
		Logger::Log("Uninitialize()");
	}

	// an "event" that is called when the application window is ready to be activated
	void OnActivated(CoreApplicationView^ coreAppView, IActivatedEventArgs^ args) {
		CoreWindow^ window = CoreWindow::GetForCurrentThread();
		window->Activate();
	}

	void PointerPressed(CoreWindow^ window, PointerEventArgs^ args){
		m_game.PointerPressed(args);
	}
	
	void PointerReleased(CoreWindow^ window, PointerEventArgs^ args){
		m_game.PointerReleased(args);
	}	
	
	void PointerMoved(CoreWindow^ window, PointerEventArgs^ args) {
		m_game.PointerMoved(args);
	}

	void PointerWheelChanged(CoreWindow^ window, PointerEventArgs^ args){
		
		// multiples of 120, positive number means scroll up, negative means scroll down
		int wheel = args->CurrentPoint->Properties->MouseWheelDelta;
		
		//MessageDialog dialog("Scrolling!", "Scrolling!");
		//dialog.ShowAsync();
	}

	void KeyDown(CoreWindow^ window, KeyEventArgs^ args) {
		m_game.KeyDown(args->VirtualKey);
	}	
	
	void KeyUp(CoreWindow^ window, KeyEventArgs^ args) {
		m_game.KeyUp(args->VirtualKey);
	}

	void Suspending(Object^ sender, SuspendingEventArgs^ args) {
		Logger::Log("Suspending()");
	}
	void Resuming(Object^ sender, Object^ args) {
		Logger::Log("Resuming()");
	}
	void Closed(CoreWindow^ sender, CoreWindowEventArgs^ args) { 
		m_windowClosed = true; 
		Logger::Log("Close()");
	}
	void VisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args) { 
		Logger::Log("VisibilityChanged()");

		// window no longer visible, so it MAY have been shut down
		if (sender->Visible == false) {
			//m_game.Finalize();
		}
	}
};

// the class definition that creates an instance of our core framework class
ref class AppSource sealed : IFrameworkViewSource {
public:
	virtual IFrameworkView^ CreateView() {
		// create an App class and return it
		return ref new App();
	}
};

[MTAThread]  // define main() as a multi-threaded-apartment function

// the starting point of all programs
int main(Array<String^>^ args) {
	// create and run a new AppSource class
	CoreApplication::Run(ref new AppSource());
	return 0;
}