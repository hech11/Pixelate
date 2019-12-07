#include "RGFpch.h"
#include "GLRenderingContext.h"


#include "RetroGF/Core/Window.h"
#include "RetroGF/Core/Application.h"

#include <GLAD/include/glad.h>
#include <GLFW/include/GLFW/glfw3.h>




namespace RGF {
	static GLFWwindow* s_WindowHandle = nullptr; // not sure on using static for this...but it works for now.

	GLRenderingContext::GLRenderingContext() {
		RGF_CORE_MSG("Creating rendering context...\n");
		m_Context = RenderingContext::ContextAPI::OPENGL;
	}
	GLRenderingContext::~GLRenderingContext() {

	}


	std::string GLRenderingContext::GetVendorName() const {
		return (const char*)glGetString(GL_VENDOR);
	}
	std::string GLRenderingContext::GetVersion() const {
		return (const char*)glGetString(GL_VERSION);
	}
	std::string GLRenderingContext::GetRendererName() const {
		return (const char*)glGetString(GL_RENDERER);
	}


	void GLRenderingContext::Init(void* window) {
		s_WindowHandle = static_cast<GLFWwindow*>(window);
		glfwMakeContextCurrent(s_WindowHandle);

		
		
		int GladStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		RGF_ASSERT(GladStatus, "Failed to load GLAD!\n");


		RGF_CORE_MSG("Rendering context : OpenGL\n\n");

		RGF_CORE_TRACE("---OpenGL Infomation---\n");
		RGF_CORE_TRACE("Vendor: %s\n", GetVendorName().c_str());
		RGF_CORE_TRACE("Version: %s\n", GetVersion().c_str());
		RGF_CORE_TRACE("Shading Language version: %s\n", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
		RGF_CORE_TRACE("GPU Card: %s\n\n\n", GetRendererName().c_str());

	}
	void GLRenderingContext::SwapBuffers() {
		glfwSwapBuffers(s_WindowHandle);
	}
}