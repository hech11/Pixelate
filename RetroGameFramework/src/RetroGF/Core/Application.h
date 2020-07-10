#pragma once

#include "RetroGF/Core/Core.h"
#include "RetroGF/Core/Window.h"

#include "RetroGF/Rendering/OrthographicCamera.h"
#include "RetroGF/Rendering/API/Shader/ShaderManager.h"

#include "RetroGF/Events/Event.h"
#include "RetroGF/Events/KeyEvents.h"
#include "RetroGF/Events/WindowEvents.h"
#include "RetroGF/Events/MouseEvents.h"



#include "RetroGF/Core/LayerStack.h"
#include "RetroGF/Imgui/ImguiLayer.h"
#include "RetroGF/Core/Timer.h"

#include "RetroGF/Debug/Instrumentor.h"

#include "RetroGF/Physics/PhysicsDebugDraw.h"



// This would be inherited from another class from CLIENT side.
// The application class is the main 'hub' of all RGF components.

// This would initialize the window, renderer, physics, etc.


namespace RGF {



	class Renderer2D;

	class RGF_API Application {

		public :
			Application();
			virtual ~Application();


			void Run();
			void Quit();

			void OnEvent(Event& e);

			void PushLayer(Layer* layer);
			void PushOverlay(Layer* overlay);

			// TODO: This is temp, may need to refactor some of these components / systems
#ifdef RGF_USE_IMGUI
			ImguiLayer& GetImguiLayer() { return *m_ImguiLayer; }
#endif
			const bool& IsMinimized() const { return m_IsMinimized; }
			const Timer& GetTime() const { return m_AppTimer; }
			const float& GetTimestep() const { return m_Timestep; }
			Window& GetWindow() { return *m_Window; }
			inline static Application& GetApp() { return *s_Instance; };
		private :
			static Application* s_Instance;
			Scoped<Window> m_Window;

			// TODO: This is temp, may need to refactor some of these components / systems
#ifdef RGF_USE_IMGUI
			void RenderImGui();
			ImguiLayer* m_ImguiLayer;
#endif


			LayerStack m_LayerStack;
			Timer m_AppTimer;
			float m_Timestep = 0.0f;

			bool m_IsRunning = true;
			bool m_IsMinimized = false;
		private :
			// Callbacks. ( Should all return bools )
			bool OnWindowClose(WindowCloseEvent& e);
			bool OnWindowResize(WindowResizeEvent& e);

	};

	// Has to be defined on CLIENT side in order to create a RGF application.
	Application* CreateApplication(); // Should return a HEAP allocated application.
}