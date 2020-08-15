#include "PXpch.h"
#include "VertexArray.h"

#include "Pixelate/Platform/OpenGL/GLVertexArray.h"
#include "Pixelate/Rendering/RendererAPI.h"

namespace Pixelate {


	Ref<VertexArray> VertexArray::Create() {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return CreateRef<GLVertexArray>();
			}

	}
}


