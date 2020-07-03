#include "RGFpch.h"
#include "VertexArray.h"

#include "RetroGF/Platform/OpenGL/GLVertexArray.h"
#include "RetroGF/Rendering/RendererAPI.h"

namespace RGF {


	Ref<VertexArray> VertexArray::Create() {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return CreateRef<GLVertexArray>();
			}

	}
}


