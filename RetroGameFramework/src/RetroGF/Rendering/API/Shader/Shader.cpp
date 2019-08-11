#include "RGFpch.h"
#include "Shader.h"

#include "RetroGF/Platform/OpenGL/GLShader.h"
#include "RetroGF/Rendering/RenderingContext.h"
#include "RetroGF/Application.h"
#include "RetroGF/Rendering/Renderer2D.h"

namespace RGF {

	
	Shader::Shader() {
		Renderer2D::GetShaderManager().Add(this, "Shader");
	}

	Shader* Shader::Create() {
		switch (RenderingContext::GetContext()) {
			case RenderingContext::ContextAPI::OPENGL:
				return new GLShader;
			}

	}



}


