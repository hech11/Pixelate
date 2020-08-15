#include "PXpch.h"
#include "Shader.h"

#include "Pixelate/Platform/OpenGL/GLShader.h"
#include "Pixelate/Rendering/RendererAPI.h"
#include "Pixelate/Core/Application.h"
#include "Pixelate/Rendering/Renderer2D.h"

namespace Pixelate {

	
	Shader::Shader() {
	}

	Ref<Shader> Shader::Create() {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return CreateRef<GLShader>();
			}

	}



}


