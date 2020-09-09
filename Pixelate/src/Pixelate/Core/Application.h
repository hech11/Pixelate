#pragma once

#include "Pixelate/Core/Core.h"
#include "Pixelate/Core/Window.h"

#include "Pixelate/Rendering/OrthographicCamera.h"
#include "Pixelate/Rendering/API/Shader/ShaderManager.h"

#include "Pixelate/Events/Event.h"
#include "Pixelate/Events/KeyEvents.h"
#include "Pixelate/Events/WindowEvents.h"
#include "Pixelate/Events/MouseEvents.h"



#include "Pixelate/Core/LayerStack.h"
#include "Pixelate/Imgui/ImguiLayer.h"
#include "Pixelate/Core/Timer.h"

#include "Pixelate/Debug/Instrumentor.h"

#include "Pixelate/Physics/PhysicsDebugDraw.h"

#include "Pixelate/Scripting/ScriptingMaster.h"

// This would be inherited from another class from CLIENT side.
// The application class is the main 'hub' of all PX components.

// This would initialize the window, renderer, physics, etc.


namespace Pixelate {



	class Renderer2D;

	class PX_API Application {

		public :
			Application();
			virtual ~Application();


			void Run();
			void Quit();

			void OnEvent(Event& e);

			void PushLayer(Layer* layer);
			void PushOverlay(Layer* overlay);

			void PopLayer(Layer* layer);
			void PopOverlay(Layer* overlay);

			// TODO: This is temp, may need to refactor some of these components / systems
#ifdef PX_USE_IMGUI
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
#ifdef PX_USE_IMGUI
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

	// Has to be defined on CLIENT side in order to create a PX application.
	Application* CreateApplication(); // Should return a HEAP allocated application.
}