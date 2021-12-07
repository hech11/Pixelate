#include "PXpch.h"
#include "Buffer.h"

#include "Pixelate/Platform/OpenGL/GLBuffer.h"

#include "Pixelate/Rendering/RendererAPI.h"

namespace Pixelate {




	// ------- Vertex buffer -------\\

	Ref<VertexBuffer> VertexBuffer::Create(const void* data, unsigned int size) {

		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return CreateRef<GLVertexBuffer>(data, size);
		}
	}
	Ref<VertexBuffer> VertexBuffer::Create(unsigned int size) {

		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::OpenGL:
			return CreateRef<GLVertexBuffer>(size);
		}
	}

	
	// ------- Index buffer -------\\

	Ref<IndexBuffer> IndexBuffer::Create(unsigned int* data, unsigned int count) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return CreateRef<GLIndexBuffer>(data, count);
		}
	}
	Ref<IndexBuffer> IndexBuffer::Create(unsigned short* data, unsigned int count) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return CreateRef<GLIndexBuffer>(data, count);
		}
	}
	Ref<IndexBuffer> IndexBuffer::Create(unsigned char* data, unsigned int count) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return CreateRef<GLIndexBuffer>(data, count);
		}
	}


	// ------- Uniform buffer -------\\

	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return CreateRef<GLUniformBuffer>(size, binding);
		}
	}

	

}