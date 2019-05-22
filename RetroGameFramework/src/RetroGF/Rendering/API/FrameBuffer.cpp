#include "RGFpch.h"
#include "FrameBuffer.h"
#include "RetroGF/Rendering/RenderingContext.h"


#include "RetroGF/Platform/OpenGL/GLFrameBuffer.h"

namespace RGF {

	FrameBuffer* FrameBuffer::Create(unsigned int width, unsigned int height) {

		switch (RenderingContext::GetContext()) {
			case RenderingContext::ContextAPI::OPENGL:
				return new GLFrameBuffer(width, height);
		}
	}


}