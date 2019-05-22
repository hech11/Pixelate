#include "RGFpch.h"
#include "VertexArray.h"

#include "RetroGF/Platform/OpenGL/GLVertexArray.h"
#include "RetroGF/Rendering/RenderingContext.h"

namespace RGF {


	VertexArray* VertexArray::Create() {
		switch (RenderingContext::GetContext()) {
			case RenderingContext::ContextAPI::OPENGL:
				return new GLVertexArray;
			}

	}
}


