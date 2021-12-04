#include "PXpch.h"
#include "Shader.h"

#include "Pixelate/Platform/OpenGL/GLShader.h"
#include "Pixelate/Rendering/RendererAPI.h"
#include "Pixelate/Core/Application.h"
#include "Pixelate/Rendering/Renderer2D.h"

namespace Pixelate {

	
	Shader::Shader() {
	}

	Ref<Shader> Shader::Create(const std::string& filepath) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return CreateRef<GLShader>(filepath);
			}

	}

	Ref<Shader> Shader::Create(const std::string& name, const char* source)
	{
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return CreateRef<GLShader>(name, source);
		}

	}



}


