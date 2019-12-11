#include "RGFpch.h"
#include "Buffer.h"

#include "RetroGF/Platform/OpenGL/GLBuffer.h"

#include "RetroGF/Rendering/RenderingContext.h"

namespace RGF {




	// ------- Vertex buffer -------\\

	Ref<VertexBuffer> VertexBuffer::Create(BufferUsage usage) {

		switch (RenderingContext::GetContext()) {
			case RenderingContext::ContextAPI::OPENGL:
				return CreateRef<GLVertexBuffer>(usage);
		}
	}


	
	// ------- Index buffer -------\\

	Ref<IndexBuffer> IndexBuffer::Create(unsigned int* data, unsigned int count) {
		switch (RenderingContext::GetContext()) {
			case RenderingContext::ContextAPI::OPENGL:
				return CreateRef<GLIndexBuffer>(data, count);
		}
	}
	Ref<IndexBuffer> IndexBuffer::Create(unsigned short* data, unsigned int count) {
		switch (RenderingContext::GetContext()) {
			case RenderingContext::ContextAPI::OPENGL:
				return CreateRef<GLIndexBuffer>(data, count);
		}
	}
	Ref<IndexBuffer> IndexBuffer::Create(unsigned char* data, unsigned int count) {
		switch (RenderingContext::GetContext()) {
			case RenderingContext::ContextAPI::OPENGL:
				return CreateRef<GLIndexBuffer>(data, count);
		}
	}


	

}