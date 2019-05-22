#include "RGFpch.h"
#include "IndexBuffer.h"

#include "RetroGF/Platform/OpenGL/GLIndexBuffer.h"
#include "RetroGF/Rendering/RenderingContext.h"


namespace RGF {


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