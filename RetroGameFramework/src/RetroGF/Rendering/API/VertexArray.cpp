#include "RGFpch.h"
#include "VertexArray.h"

#include "RetroGF/Platform/OpenGL/GLVertexArray.h"
#include "RetroGF/Rendering/RenderingContext.h"

namespace RGF {


	Ref<VertexArray> VertexArray::Create() {
		switch (RenderingContext::GetContext()) {
			case RenderingContext::ContextAPI::OPENGL:
				return std::make_shared<GLVertexArray>();
			}

	}
}


