#include "RGFpch.h"
#include "GLVertexBuffer.h"

#include <GLAD/include/glad.h>


namespace RGF {

	VertexBuffer* VertexBuffer::Create(VertexBufferUsage usage) {
		return new GLVertexBuffer(usage);
	}


	static unsigned int BufferUsageToOpenGL(VertexBufferUsage& usage) {

		switch (usage) {

			case VertexBufferUsage::Static: return GL_STATIC_DRAW;
			case VertexBufferUsage::Dynamic: return GL_DYNAMIC_DRAW;
		}

		return 0;
	}


	GLVertexBuffer::GLVertexBuffer(VertexBufferUsage usage) 
	: m_Usage(usage) 
	{
		glGenBuffers(1, &m_RendererID);
	}


	GLVertexBuffer::~GLVertexBuffer() {
		glDeleteBuffers(1, &m_RendererID);
	}



	void GLVertexBuffer::Resize(unsigned int size) {
		m_Size = size;

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, NULL, BufferUsageToOpenGL(m_Usage));
	}


	void GLVertexBuffer::SetData(unsigned int size, const void* data) {
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, data, BufferUsageToOpenGL(m_Usage));
	}


	void GLVertexBuffer::SetLayout(const VertexBufferLayout& layout) {
		m_Layout = layout;
		const auto& elements = layout.GetElements();
		unsigned int offset = 0;
		for (int i = 0; i < elements.size(); i++) {
			const auto& element = elements[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, element.count, element.type, 
				element.normilized, layout.GetStride(), (const void*)offset);
				offset += element.count * BufferElement::ConvertGLTypeToBytes(element.type);
		}
	}


	void GLVertexBuffer::Bind() {
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void GLVertexBuffer::Unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}