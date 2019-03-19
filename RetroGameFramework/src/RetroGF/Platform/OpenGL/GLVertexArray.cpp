#include "RGFpch.h"
#include "GLVertexArray.h"
#include "RetroGF/Rendering/API/VertexBufferLayout.h"

#include "GLAD/include/glad.h"

namespace RGF {

	VertexArray* VertexArray::Create(){
		return new GLVertexArray;
	}



	GLVertexArray::GLVertexArray() {
		glGenVertexArrays(1, &m_RendererID);
	}
	GLVertexArray::~GLVertexArray() {
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void GLVertexArray::PushBuffer(RGF::VertexBuffer* buffer) {
		m_Buffers.push_back(buffer);
	}

	void GLVertexArray::Bind() {
		glBindVertexArray(m_RendererID);
	}
	void GLVertexArray::Unbind() {
		glBindVertexArray(0);
	}

	void GLVertexArray::Draw(unsigned int count) {
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, NULL);
	}
}