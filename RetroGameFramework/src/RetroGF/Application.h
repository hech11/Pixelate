#pragma once

#include "RetroGF/Core.h"
#include "RetroGF/Window.h"

#include "RetroGF/Rendering/OrthographicCamera.h"
#include "RetroGF/Rendering/API/Shader/ShaderManager.h"
#include "RetroGF/Rendering/Material.h"

#include "RetroGF/Events/Event.h"
#include "RetroGF/Events/KeyEvents.h"
#include "RetroGF/Events/WindowEvents.h"



#include "RetroGF/LayerStack.h"
#include "RetroGF/Imgui/ImguiEngineEditor.h"

#include "RetroGF/Utility/Timer.h"
#include "RetroGF/Utility/File.h"


// This would be inherited from another class from CLIENT side.
// The application class is the main 'hub' of all RGF compondents.

// This would initialize the window, renderer, physics, ect.


namespace RGF {



	class Renderer2D;
	class ShaderManager;
	class MaterialManager;

	class RGF_API Application {

		public :
			Application();
			virtual ~Application();


			void Run();
			void OnEvent(Event& e);

			void PushLayer(Layer* layer);
			void PushOverlay(Layer* overlay);


			WindowImpl& GetWindow() { return *m_Window; }
			OrthographicCamera& GetCamera() { return *m_Camera; }
			FileIO& GetFileIO() { return *m_FileIO; }


#ifndef RGF_DISTRIBUTE
			ImguiEngineEditor& GetEngineEditor() { return *m_EngineEditorLayer; }
#endif

			inline static Application& GetApp() { return *s_Instance; };
		private :
			static Application* s_Instance;
			std::unique_ptr<WindowImpl> m_Window;

			// TODO: This is temp, may need to refactor some of these components / systems

			std::unique_ptr<OrthographicCamera> m_Camera;
			std::unique_ptr<FileIO> m_FileIO;
			

#ifndef RGF_DISTRIBUTE
			ImguiEngineEditor* m_EngineEditorLayer;
#endif


			LayerStack m_LayerStack;
			Timer m_AppTimer;


			bool m_IsRunning = true;
		private :
			// Callbacks. ( Should all return bools )
			bool OnWindowClose(WindowCloseEvent& e);

	};

	// Has to be defined on CLIENT side in order to create a RGF application.
	Application* CreateApplication(); // Should return a HEAP allocated application.
}