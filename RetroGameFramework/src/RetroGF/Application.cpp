#include "RGFpch.h"
#include "Application.h"


#include "Events/Event.h"
#include "Events/KeyEvents.h"

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