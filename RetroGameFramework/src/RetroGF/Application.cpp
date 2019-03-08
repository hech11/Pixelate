#include "RGFpch.h"
#include "Application.h"


#include "RetroGF/Platform/Windows/WindowsWindow.h"

#include <GLAD/include/glad.h>

namespace RGF {

	Application* Application::s_Instance = nullptr;

	Application::Application() {
		s_Instance = this;


		m_Window = std::unique_ptr<WindowImpl>(WindowImpl::Create()); // TODO: Find out if this is safe or not...it should be safe.
		m_ImguiLayer = new ImguiLayer();

		// Bind the "OnEvent" to the function pointer in "WindowImpl.h"
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		PushOverlay(m_ImguiLayer);
	}
	Application::~Application() {
	}


	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);

	}
	void Application::PushOverlay(Layer* overlay) {
		m_LayerStack.PushOverlay(overlay);
	}



	// Is bound to the function pointer in "m_Window". This function will be called when a event happens.
	void Application::OnEvent(Event& e) {

		EventDispatcher dispatcher(e);

		// Checks if the event was a "WindowCloseEvent". If it was the event, call the "OnWindowClose" function.
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));

		for (Layer* layer : m_LayerStack.GetLayerStack()) {
			layer->OnEvent(e);
			if (e.Handled)
				break;
		}

	}


	void Application::Run() {
		while (m_IsRunning) {

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (Layer* layer : m_LayerStack.GetLayerStack()) {
				layer->OnUpdate();
			}

			m_ImguiLayer->Start();
			for (Layer* layer : m_LayerStack.GetLayerStack()) {
				layer->OnImguiRender();
			}
			m_ImguiLayer->End();

			m_Window->OnUpdate();
		}
	}


	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_IsRunning = false;
		return true;
	}

}