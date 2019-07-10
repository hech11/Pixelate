#include "RGFpch.h"
#include "Buffer.h"

#include "RetroGF/Platform/OpenGL/GLBuffer.h"

#include "RetroGF/Rendering/RenderingContext.h"

namespace RGF {

	// ------- Vertex buffer -------\\


	VertexBuffer* VertexBuffer::Create(RGF::VertexBufferUsage usage) {

		switch (RenderingContext::GetContext()) {
			case RenderingContext::ContextAPI::OPENGL:
				return new GLVertexBuffer(usage);
		}
	}



	
	// ------- Index buffer -------\\

	IndexBuffer* IndexBuffer::Create(unsigned int* data, unsigned count) {
		switch (RenderingContext::GetContext()) {
			case RenderingContext::ContextAPI::OPENGL:
				return new GLIndexBuffer(data, count);
		}
	}
	IndexBuffer* IndexBuffer::Create(unsigned short* data, unsigned count) {
		switch (RenderingContext::GetContext()) {
			case RenderingContext::ContextAPI::OPENGL:
				return new GLIndexBuffer(data, count);
		}
	}
	IndexBuffer* IndexBuffer::Create(unsigned char* data, unsigned count) {
		switch (RenderingContext::GetContext()) {
			case RenderingContext::ContextAPI::OPENGL:
				return new GLIndexBuffer(data, count);
		}
	}


}