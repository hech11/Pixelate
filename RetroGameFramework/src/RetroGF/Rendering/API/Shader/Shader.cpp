#include "RGFpch.h"
#include "Shader.h"

#include "RetroGF/Platform/OpenGL/GLShader.h"
#include "RetroGF/Rendering/RendererAPI.h"
#include "RetroGF/Core/Application.h"
#include "RetroGF/Rendering/Renderer2D.h"

namespace RGF {

	
	Shader::Shader() {
	}

	Ref<Shader> Shader::Create() {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return CreateRef<GLShader>();
			}

	}



}


