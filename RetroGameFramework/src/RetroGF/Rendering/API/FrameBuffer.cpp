#include "RGFpch.h"
#include "FrameBuffer.h"

#include "RetroGF/Rendering/RendererAPI.h"
#include "RetroGF/Platform/OpenGL/GLFrameBuffer.h"


namespace RGF {

	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecs& specs) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return CreateRef<GLFrameBuffer>(specs);
		}
	}


}