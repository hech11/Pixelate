#include "RGFpch.h"
#include "VertexBuffer.h"

#include "RetroGF/Platform/OpenGL/GLVertexBuffer.h"
#include "RetroGF/Rendering/RenderingContext.h"

namespace RGF {

	VertexBuffer* VertexBuffer::Create(RGF::VertexBufferUsage usage) {

		switch (RenderingContext::GetContext()) {
		case RenderingContext::ContextAPI::OPENGL:
			return new GLVertexBuffer(usage);
		}
	}

}