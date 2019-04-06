#include "RGFpch.h"
#include "GLIndexBuffer.h"

#include "GLCommon.h"




namespace RGF {

	// TODO: In the future this 'Create' may want to be in the implementation class and not hardcoded into here.
	// TODO: There is no way to switch between other API's at the moment if i wanted to.
	IndexBuffer* IndexBuffer::Create(unsigned int* data, unsigned count) {
		return new GLIndexBuffer(data, count);
	}
	IndexBuffer* IndexBuffer::Create(unsigned short* data, unsigned count) {
		return new GLIndexBuffer(data, count);
	}
	IndexBuffer* IndexBuffer::Create(unsigned char* data, unsigned count) {
		return new GLIndexBuffer(data, count);
	}

	GLIndexBuffer::GLIndexBuffer(unsigned int* data, unsigned int count) : m_Count(count) {
		GLCall(glGenBuffers(1, &m_RendererID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
	}
	GLIndexBuffer::GLIndexBuffer(unsigned short* data, unsigned int count) : m_Count(count) {
		GLCall(glGenBuffers(1, &m_RendererID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned short), data, GL_STATIC_DRAW));
	}
	GLIndexBuffer::GLIndexBuffer(unsigned char* data, unsigned int count) : m_Count(count) {
		GLCall(glGenBuffers(1, &m_RendererID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned char), data, GL_STATIC_DRAW));

	}

	GLIndexBuffer::~GLIndexBuffer() {
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}

	void GLIndexBuffer::Bind() const {
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	}
	void GLIndexBuffer::Unbind() const {
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	}


	unsigned int GLIndexBuffer::GetCount() const {
		return m_Count;
	}

}