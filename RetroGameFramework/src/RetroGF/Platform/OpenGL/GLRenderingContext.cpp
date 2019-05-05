#include "RGFpch.h"
#include "GLRenderingContext.h"


#include "RetroGF/WindowImpl.h"
#include "RetroGF/Application.h"

#include <GLAD/include/glad.h>
#include <GLFW/include/GLFW/glfw3.h>

namespace RGF {
	

	GLRenderingContext::GLRenderingContext() {
		m_Context = RenderingContext::ContextAPI::OPENGL;
	}
	GLRenderingContext::~GLRenderingContext() {

	}

	void GLRenderingContext::Init(void* window) {
		m_WindowHandle = static_cast<GLFWwindow*>(window);
		glfwMakeContextCurrent(m_WindowHandle);

		int GladStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		if (!GladStatus) {
			RGF_CORE_ERROR("Failed to load GLAD!\n");
		}
	}
	void GLRenderingContext::SwapBuffers() {
		glfwSwapBuffers(m_WindowHandle);
	}
}