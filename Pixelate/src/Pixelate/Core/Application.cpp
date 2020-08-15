#include "PXpch.h"
#include "Application.h"


#include "Pixelate/Platform/Windows/WindowsWindow.h"

#include <Pixelate/Rendering/API/Buffer.h>
#include <Pixelate/Rendering/API/VertexArray.h>
#include <Pixelate/Rendering/API/Texture.h>


#include <GLM/glm/gtc/matrix_transform.hpp>
#include <Pixelate/Core/Random.h>

#include <Pixelate/Rendering/Renderer2D.h>
#include <Pixelate/Rendering/RenderCommand.h>

#include "Pixelate/Platform/OpenGL/GLCommon.h"

#include "Pixelate/Core/Input.h"
#include "Pixelate/Core/KeyCodes.h"
#include "Pixelate/Core/MouseButtonCodes.h"

#include "Pixelate/Audio/Audio.h"

#include "Pixelate/Physics/Physics.h"

#ifdef PX_USE_IMGUI
#include <IMGUI/imgui.h>
#include <IMGUI/examples/imgui_impl_glfw.h>
#include <IMGUI/examples/imgui_impl_opengl3.h>

#endif


namespace Pixelate {




	Application* Application::s_Instance = nullptr;

	

	Application::Application() {
		PX_PROFILE_FUNCTION();
		s_Instance = this;

		m_Window = Scoped<Window>(Window::Create({960,540}));
		// Bind the "OnEvent" to the function pointer in "WindowImpl.h"
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
#ifdef PX_USE_IMGUI
		m_ImguiLayer = new ImguiLayer;
		ImGui::InstallImguiCallbacks(std::bind(&Application::OnEvent, this, std::placeholders::_1));
#endif




		Renderer2D::Init();
		Audio::Init();

		// TODO: not ideal at all. fix this
		#ifdef PX_DEBUG
		ScriptingMaster::Init("../bin/Debug-windows-x86_64/Sandbox-CSharpApp/Sandbox-CSharpApp.dll");
		#endif

		#ifdef PX_RELEASE
		ScriptingMaster::Init("../bin/Release-windows-x86_64/Sandbox-CSharpApp/Sandbox-CSharpApp.dll");
		#endif

		#ifdef PX_DISTRIBUTE
		#error "Dist builds do not support scripting!"
		#endif


		PX_CORE_WARN("Time took to init application: %fms\n", m_AppTimer.GetElapsedMillis());


	}
	Application::~Application() {
		PX_PROFILE_FUNCTION();
		Renderer2D::ShutDown();
		ScriptingMaster::Shutdown();
	}


	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);

	}
	void Application::PushOverlay(Layer* overlay) {
		m_LayerStack.PushOverlay(overlay);
	}


	// Is bound to the function pointer in "m_Window". This function will be called when a event happens.
	void Application::OnEvent(Event& e) {
		PX_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);

		// Checks if the event was a "WindowCloseEvent". If it was the event, call the "OnWindowClose" function.
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&Application::OnWindowResize, this, std::placeholders::_1));

		for (Layer* layer : m_LayerStack.GetLayerStack()) {
			layer->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::RenderImGui() {
		PX_PROFILE_SCOPE("Application::ImguiRender||EngineEditor");
		m_ImguiLayer->Start();
			for (Layer* layer : m_LayerStack.GetLayerStack()) {
				PX_PROFILE_SCOPE("Application::Layer::" + layer->GetName() + "::OnImguiRender");
				layer->OnImguiRender();
			}
		m_ImguiLayer->End();
	}


	void Application::Run() {
		PX_PROFILE_FUNCTION();

		
		float Time = 0.0f;

		float LastTime = 0.0f;

		m_AppTimer.Reset();
		while (m_IsRunning) {
			PX_PROFILE_SCOPE("Application::Run::m_IsRunning::Loop");
			float time = m_AppTimer.GetElapsedSeconds();
			m_Timestep = time - LastTime;
			LastTime = time;


			if (!m_IsMinimized) {

				for (Layer* layer : m_LayerStack.GetLayerStack()) {

					PX_PROFILE_SCOPE("Application::Layer::" + layer->GetName() + "::OnUpdate");

					layer->OnUpdate(m_Timestep);
				}


			}

			ScriptingMaster::OnUpdate();
	#ifdef PX_USE_IMGUI
			if (!m_IsMinimized) {
				RenderImGui();
	
			}

	#endif

			m_Window->OnUpdate();
			if (m_AppTimer.GetElapsedSeconds() - Time > 1.0f) {
				PX_CORE_MSG("%f: timestep\n", m_Timestep *1000.0f);
				Time += 1.0f;

#ifdef PX_DISTRIBUTE
				printf("%f: timestep\n", m_Timestep *1000.0f);
#endif
			}
		}

	}


	bool Application::OnWindowClose(WindowCloseEvent& e) {
		Quit();
		return true;
	}
	bool Application::OnWindowResize(WindowResizeEvent& e) {
		if (e.GetWidth() == 0 && e.GetHeight() == 0) {
			m_IsMinimized = true;
		} else {
			m_IsMinimized = false;
			RenderCommand::SetViewport(0, 0, e.GetWidth(), e.GetHeight());
		}

		return true;
	}
	void Application::Quit() {
		m_IsRunning = false;
	}


}