#include "RGFpch.h"
#include "Application.h"


#include "RetroGF/Platform/Windows/WindowsWindow.h"

#include <RetroGF/Rendering/API/Buffer.h>
#include <RetroGF/Rendering/API/VertexArray.h>
#include <RetroGF/Rendering/API/Texture.h>


#include <GLM/glm/gtc/matrix_transform.hpp>
#include <RetroGF/Utility/Random.h>

#include <RetroGF/Rendering/Renderer2D.h>
#include <RetroGF/Rendering/RenderCommand.h>

#include "RetroGF/Platform/OpenGL/GLCommon.h"

#include "RetroGF/Core/Input.h"
#include "RetroGF/Core/KeyCodes.h"
#include "RetroGF/Core/MouseButtonCodes.h"

#include "RetroGF/Audio/Audio.h"

#include "RetroGF/Physics/Physics.h"

#ifdef RGF_USE_IMGUI
#include <IMGUI/imgui.h>
#include <IMGUI/examples/imgui_impl_glfw.h>
#include <IMGUI/examples/imgui_impl_opengl3.h>

#endif


namespace RGF {




	Application* Application::s_Instance = nullptr;

	

	Application::Application() {
		RGF_PROFILE_FUNCTION();
		s_Instance = this;

		m_Window = Scoped<WindowImpl>(WindowImpl::Create({960,540}));
		// Bind the "OnEvent" to the function pointer in "WindowImpl.h"
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
#ifdef RGF_USE_IMGUI
		m_ImguiLayer = new ImguiLayer;
		ImGui::InstallImguiCallbacks(std::bind(&Application::OnEvent, this, std::placeholders::_1));
#endif




		RGF_CORE_MSG("Creating the renderer!\n");
		Renderer2D::Init();

		RGF_CORE_MSG("initializing the audio!\n");
		Audio::Init();
		RGF_CORE_MSG("initializing physics!\n");
		Physics::Init();

		RGF_CORE_TRACE("RGF application created!\n");
		RGF_CORE_TRACE("Time took to init application: %fms\n", m_AppTimer.GetElapsedMillis());





		

	}
	Application::~Application() {
		RGF_PROFILE_FUNCTION();
		Renderer2D::ShutDown();
	}


	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);

	}
	void Application::PushOverlay(Layer* overlay) {
		m_LayerStack.PushOverlay(overlay);
	}


	// Is bound to the function pointer in "m_Window". This function will be called when a event happens.
	void Application::OnEvent(Event& e) {
		RGF_PROFILE_FUNCTION();

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


	void Application::Run() {
		RGF_PROFILE_FUNCTION();

		
		float Time = 0.0f;

		float LastTime = 0.0f;

		m_AppTimer.Reset();
		while (m_IsRunning) {
			RGF_PROFILE_SCOPE("Application::Run::m_IsRunning::Loop");
			float time = m_AppTimer.GetElapsedSeconds();
			float timeStep = time - LastTime;
			LastTime = time;


			Physics::Update(m_AppTimer.GetElapsedSeconds());

			for (Layer* layer : m_LayerStack.GetLayerStack()) {

				RGF_PROFILE_SCOPE("Application::Layer::" + layer->GetName() + "::OnUpdate");

				layer->OnUpdate(timeStep);
			}


#ifdef RGF_USE_IMGUI

			{
				RGF_PROFILE_SCOPE("Application::ImguiRender||EngineEditor");
				m_ImguiLayer->Start();
				for (Layer* layer : m_LayerStack.GetLayerStack()) {
					RGF_PROFILE_SCOPE("Application::Layer::" + layer->GetName() + "::OnImguiRender");
					layer->OnImguiRender();
				}
				m_ImguiLayer->End();
			}
#endif


			m_Window->OnUpdate();

			if (m_AppTimer.GetElapsedSeconds() - Time > 1.0f) {
				Time += 1.0f;
				RGF_CORE_MSG("%f: timestep\n", timeStep*1000.0f);

#ifdef RGF_DISTRIBUTE
				printf("%f: timestep\n", timeStep*1000.0f);
#endif
			}
		}

	}


	bool Application::OnWindowClose(WindowCloseEvent& e) {
		Quit();
		return true;
	}
	bool Application::OnWindowResize(WindowResizeEvent& e) {
		RenderCommand::SetViewport(0, 0, e.GetWidth(), e.GetHeight());
		return true;
	}
	void Application::Quit() {
		m_IsRunning = false;
	}


}