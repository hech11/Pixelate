#include "RGFpch.h"
#include "Buffer.h"

#include "RetroGF/Platform/OpenGL/GLBuffer.h"

#include "RetroGF/Rendering/RenderingContext.h"

namespace RGF {




	// ------- Vertex buffer -------\\

	VertexBuffer* VertexBuffer::Create(unsigned int size, const void* data, BufferUsage usage) {

		switch (RenderingContext::GetContext()) {
			case RenderingContext::ContextAPI::OPENGL:
				return new GLVertexBuffer(size, data, usage);
		}
	}


	
	// ------- Index buffer -------\\

	IndexBuffer* IndexBuffer::Create(unsigned int* data, unsigned int count) {
		switch (RenderingContext::GetContext()) {
			case RenderingContext::ContextAPI::OPENGL:
				return new GLIndexBuffer(data, count);
		}
	}
	IndexBuffer* IndexBuffer::Create(unsigned short* data, unsigned int count) {
		switch (RenderingContext::GetContext()) {
			case RenderingContext::ContextAPI::OPENGL:
				return new GLIndexBuffer(data, count);
		}
	}
	IndexBuffer* IndexBuffer::Create(unsigned char* data, unsigned int count) {
		switch (RenderingContext::GetContext()) {
			case RenderingContext::ContextAPI::OPENGL:
				return new GLIndexBuffer(data, count);
		}
	}


	

}