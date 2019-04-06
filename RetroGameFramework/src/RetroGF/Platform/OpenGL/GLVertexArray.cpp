#include "RGFpch.h"
#include "GLVertexArray.h"
#include "RetroGF/Rendering/API/VertexBufferLayout.h"

#include "GLCommon.h"

namespace RGF {

	// TODO: In the future this 'Create' may want to be in the implementation class and not hardcoded into here.
	// TODO: There is no way to switch between other API's at the moment if i wanted to.
	VertexArray* VertexArray::Create(){
		return new GLVertexArray;
	}



	GLVertexArray::GLVertexArray() {
		GLCall(glGenVertexArrays(1, &m_RendererID));
	}
	GLVertexArray::~GLVertexArray() {
		GLCall(glDeleteVertexArrays(1, &m_RendererID));
	}

	void GLVertexArray::PushBuffer(RGF::VertexBuffer* buffer) {
		m_Buffers.push_back(buffer);
	}

	void GLVertexArray::Bind() const {
		GLCall(glBindVertexArray(m_RendererID));
	}
	void GLVertexArray::Unbind() const {
		GLCall(glBindVertexArray(0));
	}

	void GLVertexArray::Draw(unsigned int count) {
		GLCall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, NULL));
	}
}