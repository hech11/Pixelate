#include "PXpch.h"
#include "Pixelate/Platform/Windows/WindowsWindow.h"

#include "Pixelate/Events/Event.h"
#include "Pixelate/Events/WindowEvents.h"
#include "Pixelate/Events/KeyEvents.h"
#include "Pixelate/Events/MouseEvents.h"


#include <glad/glad.h>
#include <GLFW/include/GLFW/glfw3.h>

#include "Pixelate/Debug/Instrumentor.h"
#include "STB/IMAGE/stb_image.h"



namespace Pixelate {



	// TODO: A "LibaryManager" could init GLFW instead of initializing it in "StartUp".
	// TODO: A "LibaryManager" could shutdown GLFW instead of terminating it in "ShutDown".



	Window* Window::Create(const WindowProps& props) {
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
		PX_PROFILE_FUNCTION();

		m_Data.props = props;

		int glfwValidation = glfwInit();
		PX_ASSERT(glfwValidation, "Failed to init GLFW!\n");
		
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

		m_Window = glfwCreateWindow(m_Data.props.Width, m_Data.props.Height, m_Data.props.Title.c_str(), nullptr, nullptr);

		PX_ASSERT(m_Window, "Failed to create the window");

		m_Context = Pixelate::RenderingContext::CreateContext(Pixelate::RenderingContext::ContextAPI::OPENGL);
		m_Context->Init(m_Window);


		SetVsync(false);

		glfwSetWindowPos(m_Window, m_Data.props.xPos, m_Data.props.yPos);
		glfwSetWindowUserPointer(m_Window, &m_Data); // Used to access "m_Data" for sending the events to "OnEvent" in "Application".
		
		glfwMaximizeWindow(m_Window);
		GLFWimage icon[1];
		int bbp;
		icon[0].pixels = stbi_load("resources/icons/window/Pixelate_Logo.png", &icon[0].width, &icon[0].height, &bbp, 4);
		glfwSetWindowIcon(m_Window, 1, icon);
		stbi_image_free(icon[0].pixels);

		// Setting glfw callbacks.
		// First create the event, set this window's variables to the callbacks data, call the "OnEvent" function in "Application" and pass the created event by ref.

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			// *Example of setting a callback*.

			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window); // Get "m_Data".


			WindowResizeEvent event(width, height); // Create event and pass in the data GLFW supplies (in the lambda).

			// Assign the "WindowsWindow" data to the data supplied by GLFW (in the lambda).
			data.props.Width = width;
			data.props.Height = height;

			// Call the "OnEvent" function in "Application" and pass by ref the current event.
			data.EventCallback(event);
		});
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent event;

			data.EventCallback(event);
		});
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {

			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT: 
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;

				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;

				}
			}
		});
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xMousePos, double yMousePos) {
			
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((int)xMousePos, (int)yMousePos);
			data.EventCallback(event);
		});
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {

			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button, 0);
					data.EventCallback(event);
					break;

				}
				case GLFW_REPEAT:
				{
					MouseButtonPressedEvent event(button, 1);
					data.EventCallback(event);
					break;
				}
					
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xScroll, double yScroll) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((int)xScroll, (int)yScroll);
			data.EventCallback(event);
		});
		glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int xPos, int yPos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowMovedEvent event(xPos, yPos);
			data.EventCallback(event);

			data.props.xPos = xPos;
			data.props.yPos = yPos;
		});
		glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			if (focused) {
				WindowGainedFocusEvent event;
				data.EventCallback(event);
			} else {
				WindowLosedFocusEvent event;
				data.EventCallback(event);
			}

		});


	}




	void WindowsWindow::ShutDown() {

		glfwDestroyWindow(m_Window);
		glfwTerminate();
		PX_CORE_TRACE("Shutting down window!\n");

	}



	void WindowsWindow::OnUpdate() {

		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	

	void WindowsWindow::SetVsync(bool enabled) { 
		if(enabled) {
			glfwSwapInterval(1);
		} else {
			glfwSwapInterval(0);
		}
	}

	void WindowsWindow::SetTitle(const std::string& title) {
		glfwSetWindowTitle(m_Window, title.c_str());
		m_Data.props.Title = title;
	}




}