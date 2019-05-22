#include "RGFpch.h"
#include "Shader.h"

#include "RetroGF/Platform/OpenGL/GLShader.h"
#include "RetroGF/Rendering/RenderingContext.h"

namespace RGF {


	Shader* Shader::Create() {
		switch (RenderingContext::GetContext()) {
			case RenderingContext::ContextAPI::OPENGL:
				return new GLShader;
			}

	}
}


