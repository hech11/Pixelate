#include "RGFpch.h"
#include "GLRenderingContext.h"


#include "RetroGF/WindowImpl.h"
#include "RetroGF/Application.h"

#include <GLAD/include/glad.h>
#include <GLFW/include/GLFW/glfw3.h>

namespace RGF {
	

	GLRenderingContext::GLRenderingContext() {
		RGF_CORE_MSG("Creating rendering context...\n");
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

		RGF_CORE_MSG("Rendering context : OpenGL\n\n");

		RGF_CORE_TRACE("---OpenGL Infomation---\n");
		RGF_CORE_TRACE("Vendor: %s\n", (const char*)glGetString(GL_VENDOR));
		RGF_CORE_TRACE("Version: %s\n", (const char*)glGetString(GL_VERSION));
		RGF_CORE_TRACE("Shading Language version: %s\n", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
		RGF_CORE_TRACE("GPU Card: %s\n\n\n", (const char*)glGetString(GL_RENDERER));

	}
	void GLRenderingContext::SwapBuffers() {
		glfwSwapBuffers(m_WindowHandle);
	}
}