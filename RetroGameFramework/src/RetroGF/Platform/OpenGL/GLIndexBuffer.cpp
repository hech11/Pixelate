#include "RGFpch.h"
#include "GLIndexBuffer.h"

#include <GLAD/include/glad.h>

namespace RGF {


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
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
	}
	GLIndexBuffer::GLIndexBuffer(unsigned short* data, unsigned int count) {
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned short), data, GL_STATIC_DRAW);
	}
	GLIndexBuffer::GLIndexBuffer(unsigned char* data, unsigned int count) {
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned char), data, GL_STATIC_DRAW);

	}

	GLIndexBuffer::~GLIndexBuffer() {
		glDeleteBuffers(1, &m_RendererID);
	}

	void GLIndexBuffer::Bind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}
	void GLIndexBuffer::Unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}


	unsigned int GLIndexBuffer::GetCount() const {
		return m_Count;
	}

}