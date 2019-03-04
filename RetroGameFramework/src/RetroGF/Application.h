#pragma once

#include "RetroGF/Core.h"
#include "RetroGF/WindowImpl.h"
#include "RetroGF/Platform/Windows/WindowsWindow.h"


#include "Events/Event.h"
#include "Events/KeyEvents.h"
#include "Events/WindowEvents.h"


// This would be inherited from another class from CLIENT side.
// The application class is the main 'hub' of all RGF compondents.

// This would initialize the window, renderer, physics, ect.



namespace RGF {

	class RGF_API Application {

		public :
			Application();
			virtual ~Application();


			void Run();
			void OnEvent(Event& e);


		private :
			std::unique_ptr<WindowImpl> m_Window;
			bool m_IsRunning = true;

		private :
			// Callbacks. ( Should all return bools )
			bool OnWindowClose(WindowCloseEvent& e);

	};

	// Has to be defined on CLIENT side in order to create a RGF application.
	Application* CreateApplication(); // Should return a HEAP allocated application.
}