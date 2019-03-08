#pragma once

#include "RetroGF/Core.h"
#include "RetroGF/WindowImpl.h"


#include "RetroGF/Events/Event.h"
#include "RetroGF/Events/KeyEvents.h"
#include "RetroGF/Events/WindowEvents.h"



#include "RetroGF/LayerStack.h"
#include "RetroGF/Imgui/ImguiLayer.h"


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

			void PushLayer(Layer* layer);
			void PushOverlay(Layer* overlay);

			WindowImpl& GetWindow() { return *m_Window; }
			inline static Application& GetApp() { return *s_Instance; };
		private :
			std::unique_ptr<WindowImpl> m_Window;
			ImguiLayer* m_ImguiLayer;
			LayerStack m_LayerStack;

			bool m_IsRunning = true;
			static Application* s_Instance;

		private :
			// Callbacks. ( Should all return bools )
			bool OnWindowClose(WindowCloseEvent& e);

	};

	// Has to be defined on CLIENT side in order to create a RGF application.
	Application* CreateApplication(); // Should return a HEAP allocated application.
}