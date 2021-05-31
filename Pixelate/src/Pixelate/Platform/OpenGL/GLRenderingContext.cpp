#include "PXpch.h"
#include "GLRenderingContext.h"


#include "Pixelate/Core/Window.h"
#include "Pixelate/Core/Application.h"

#include "Pixelate/Rendering/RenderCommand.h"
#include <glad/glad.h>
#include <GLFW/include/GLFW/glfw3.h>

#include "Pixelate/Rendering/RendererAPI.h"




namespace Pixelate {
	static GLFWwindow* s_WindowHandle = nullptr;

	GLRenderingContext::GLRenderingContext() 
		: m_Context(RenderingContext::ContextAPI::OPENGL)
	{
	}



	void GLRenderingContext::Init(void* window) {
		s_WindowHandle = static_cast<GLFWwindow*>(window);
		glfwMakeContextCurrent(s_WindowHandle);
		
		
		int GladStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		PX_ASSERT(GladStatus, "Failed to load GLAD!\n");


		RenderCommand::Init(RenderingContext::ContextAPI::OPENGL);

	}
	void GLRenderingContext::SwapBuffers() {
		glfwSwapBuffers(s_WindowHandle);
	}
}