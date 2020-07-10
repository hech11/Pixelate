#include "RGFpch.h"
#include "GLBuffer.h"

#include "GLCommon.h"

#include "RetroGF/Rendering/Renderer2D.h"

namespace RGF {



	unsigned int GetBufferPlatformUsage(BufferUsage usage) {
		switch (usage) {

			case BufferUsage::Static: return GL_STATIC_DRAW;
			case BufferUsage::Dynamic: return GL_DYNAMIC_DRAW;
		}

		return 0;

	}



	// ------- Vertex buffer -------\\

	GLVertexBuffer::GLVertexBuffer(const void* data, unsigned int size) 
	: m_Stats({size, BufferUsage::Static })
	{


		GLCall(glGenBuffers(1, &m_RendererID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, m_Stats.Size, data, GL_STATIC_DRAW));

	}


	GLVertexBuffer::GLVertexBuffer(unsigned int size)
		: m_Stats({ size, BufferUsage::Dynamic })
	{


		GLCall(glGenBuffers(1, &m_RendererID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, m_Stats.Size, nullptr, GL_DYNAMIC_DRAW));


	}

	GLVertexBuffer::~GLVertexBuffer() {
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}


	// TODO: it would be nice to have an offset, future API refactor?
	void GLVertexBuffer::SetData(void* data, unsigned int size) {
		
		if (m_Stats.Usage == BufferUsage::Dynamic)
			m_Stats.Size = size;
		else {
			RGF_CORE_WARN("Trying to change the data of a vertex buffer while it is set to static draw!\n");
		}


		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_Stats.Size, data);

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

	GLIndexBuffer::GLIndexBuffer(unsigned int* data, unsigned int count) : m_Count(count), m_Type(GL_UNSIGNED_INT)
	{

		GLCall(glGenBuffers(1, &m_RendererID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned int), data, GL_STATIC_DRAW));
	}
	GLIndexBuffer::GLIndexBuffer(unsigned short* data, unsigned int count) : m_Count(count), m_Type(GL_UNSIGNED_SHORT) 
	{

		GLCall(glGenBuffers(1, &m_RendererID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned short), data, GL_STATIC_DRAW));

	}
	GLIndexBuffer::GLIndexBuffer(unsigned char* data, unsigned int count) : m_Count(count), m_Type(GL_UNSIGNED_BYTE) 
	{

		GLCall(glGenBuffers(1, &m_RendererID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned char), data, GL_STATIC_DRAW));
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