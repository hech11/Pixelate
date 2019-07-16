#include "RGFpch.h"
#include "GLBuffer.h"

#include "GLCommon.h"

namespace RGF {



	unsigned int GetBufferPlatformUsage(BufferUsage usage) {
		switch (usage) {

			case BufferUsage::Static: return GL_STATIC_DRAW;
			case BufferUsage::Dynamic: return GL_DYNAMIC_DRAW;
		}

		return 0;

	}



	// ------- Vertex buffer -------\\

	GLVertexBuffer::GLVertexBuffer(BufferUsage usage) {
		m_Usage = usage;
		GLCall(glGenBuffers(1, &m_RendererID));
	}


	GLVertexBuffer::~GLVertexBuffer() {
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}



	void GLVertexBuffer::Resize(unsigned int size) {
		m_Size = size;
	}


	void GLVertexBuffer::SetData(const void* data) {
		if(!m_Size) RGF_CORE_WARN("VertexBuffer's size is set to 0!\n");

		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, m_Size, data, GetBufferPlatformUsage(m_Usage)));
	}


	void GLVertexBuffer::SetLayout(const BufferLayout& layout) {
		m_Layout = layout;
	}


	void GLVertexBuffer::Bind() const {
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	}

	void GLVertexBuffer::Unbind() const {
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}






	// ------- Index buffer -------\\

	GLIndexBuffer::GLIndexBuffer(unsigned int* data, unsigned int count) {
		m_Count = count;
		GLCall(glGenBuffers(1, &m_RendererID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
	}
	GLIndexBuffer::GLIndexBuffer(unsigned short* data, unsigned int count) {
		m_Count = count;
		GLCall(glGenBuffers(1, &m_RendererID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned short), data, GL_STATIC_DRAW));
	}
	GLIndexBuffer::GLIndexBuffer(unsigned char* data, unsigned int count) {
		m_Count = count;
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


}