#include "PXpch.h"
#include "FrameBuffer.h"

#include "Pixelate/Rendering/RendererAPI.h"
#include "Pixelate/Platform/OpenGL/GLFrameBuffer.h"


namespace Pixelate {

	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecs& specs) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return CreateRef<GLFrameBuffer>(specs);
		}
	}


}