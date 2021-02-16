#include "pch.h"
#include "Game.h"

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
	}

	virtual void SetWindow(CoreWindow^ window){
		window->PointerPressed += ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &App::PointerPressed);
		window->PointerWheelChanged += ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &App::PointerWheelChanged);
		window->KeyDown += ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &App::KeyDown);
		window->KeyUp += ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &App::KeyUp);

		window->Closed += ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &App::Closed);
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

	}
	virtual void Uninitialize() {}

	// an "event" that is called when the application window is ready to be activated
	void OnActivated(CoreApplicationView^ coreAppView, IActivatedEventArgs^ args) {
		CoreWindow^ window = CoreWindow::GetForCurrentThread();
		window->Activate();
	}

	void PointerPressed(CoreWindow^ window, PointerEventArgs^ args){
		MessageDialog dialog("Thank you for pressing!", "Notice!");
		dialog.ShowAsync();
	}	
	
	void PointerWheelChanged(CoreWindow^ window, PointerEventArgs^ args){
		
		// multiples of 120, positive number means scroll up, negative means scroll down
		int wheel = args->CurrentPoint->Properties->MouseWheelDelta;
		
		MessageDialog dialog("Scrolling!", "Scrolling!");
		dialog.ShowAsync();
	}

	void KeyDown(CoreWindow^ window, KeyEventArgs^ args) {
		if (args->VirtualKey == VirtualKey::A) {
			MessageDialog dialog("A pressed!", "A pressed!");
			//dialog.ShowAsync();
		}
	}	
	
	void KeyUp(CoreWindow^ window, KeyEventArgs^ args) {
		if (args->VirtualKey == VirtualKey::A) {
			MessageDialog dialog("A released!", "A released!");
			dialog.ShowAsync();
		}
	}

	void Suspending(Object^ sender, SuspendingEventArgs^ args) {}
	void Resuming(Object^ sender, Object^ args) {}
	void Closed(CoreWindow^ sender, CoreWindowEventArgs^ args) { m_windowClosed = true; }
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