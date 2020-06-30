#include "RGFpch.h"
#include "FrameBuffer.h"

#include "RetroGF/Rendering/RenderingContext.h"
#include "RetroGF/Platform/OpenGL/GLFrameBuffer.h"


namespace RGF {

	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecs& specs) {
		switch (RenderingContext::GetContext()) {
			case RenderingContext::ContextAPI::OPENGL:
				return CreateRef<GLFrameBuffer>(specs);
		}
	}


}