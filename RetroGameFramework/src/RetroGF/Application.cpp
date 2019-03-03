#include "RGFpch.h"
#include "Application.h"


namespace RGF {

	Application::Application() {

		// TODO: Find out if this is safe or not...it should be safe.
		Window = std::unique_ptr<WindowImpl>(WindowImpl::Create());

	}
	Application::~Application() {

	}


	void Application::Run() {
		while (true) {


			Window.get()->OnUpdate();
		}
	}

}