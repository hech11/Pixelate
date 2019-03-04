#include "RGFpch.h"
#include "Application.h"



namespace RGF {

	Application::Application() {

		// TODO: Find out if this is safe or not...it should be safe.
		m_Window = std::unique_ptr<WindowImpl>(WindowImpl::Create());

		// Bind the "OnEvent" to the function pointer in "WindowImpl.h"
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	}
	Application::~Application() {

	}


	// Is bound to the function pointer in "m_Window". This function will be called when a event happens.
	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);

		// Checks if the event was a "WindowCloseEvent". If it was the event, call the "OnWindowClose" function.
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));

		RGF_CORE_MSG("%s\n", e.ToString().c_str());
	}


	void Application::Run() {
		while (m_IsRunning) {


			m_Window->OnUpdate();
		}
	}


	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_IsRunning = false;
		return true;
	}

}