#include "RGFpch.h"
#include "GLRenderingContext.h"


#include "RetroGF/Core/Window.h"
#include "RetroGF/Core/Application.h"

#include "RetroGF/Rendering/RenderCommand.h"
#include <GLAD/include/glad.h>
#include <GLFW/include/GLFW/glfw3.h>

#include "RetroGF/Rendering/RendererAPI.h"




namespace RGF {
	static GLFWwindow* s_WindowHandle = nullptr;

	GLRenderingContext::GLRenderingContext() 
		: m_Context(RenderingContext::ContextAPI::OPENGL)
	{
		RGF_CORE_MSG("Creating rendering context...\n");
	}



	void GLRenderingContext::Init(void* window) {
		s_WindowHandle = static_cast<GLFWwindow*>(window);
		glfwMakeContextCurrent(s_WindowHandle);
		
		
		int GladStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		RGF_ASSERT(GladStatus, "Failed to load GLAD!\n");


		RenderCommand::Init(RenderingContext::ContextAPI::OPENGL);

	}
	void GLRenderingContext::SwapBuffers() {
		glfwSwapBuffers(s_WindowHandle);
	}
}