#include "RGFpch.h"
#include "RetroGF/Platform/Windows/WindowsWindow.h"


namespace RGF {



	// TODO: A "LibaryManager" could init GLFW instead of initializing it in "StartUp".
	// TODO: A "LibaryManager" could shutdown GLFW instead of terminating it in "ShutDown".

	// TODO: Create and replace "RGF_CORE_ERROR" with an RGF_ASSERT.
	// TODO: Set up the event callbacks.



	// This was delcared from the BASE class (WindowsImpl.h).
	WindowImpl* WindowImpl::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}


	WindowsWindow::WindowsWindow(const WindowProps& props) {
		StartUp(props);
	}
	WindowsWindow::~WindowsWindow() {
		ShutDown();
	}

	// Init Window and event callbacks.
	void WindowsWindow::StartUp(const WindowProps& props) {
		m_Data.props = props;


		if (!glfwInit()) {
			glfwTerminate();
			RGF_CORE_ERROR("Failed to init GLFW!\n");
			return;
		}


		m_Window = glfwCreateWindow(m_Data.props.Width, m_Data.props.Height, m_Data.props.Title.c_str(), nullptr, nullptr);
		if (m_Window == nullptr) {
			glfwTerminate();
			RGF_CORE_ERROR("Failed to create the window!\n");
			return;
		}

		glfwMakeContextCurrent(m_Window);
		SetVsync(true);


		
		glfwSetWindowUserPointer(m_Window, this); // Used to access "m_Data" for dispatching events.

		


		RGF_CORE_TRACE("Created window!\n");

	}




	void WindowsWindow::ShutDown() {

		glfwDestroyWindow(m_Window);
		glfwTerminate();
		RGF_CORE_TRACE("Shutting down window!\n");

	}



	void WindowsWindow::OnUpdate() {

		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	

	void WindowsWindow::SetVsync(bool enabled) { // Could be a better way to implement this.

		if(enabled) {
			glfwSwapInterval(1);
		} else {
			glfwSwapInterval(0);
		}
	}



}