#include "RGFpch.h"
#include "GLVertexArray.h"
#include "RetroGF/Rendering/API/VertexBufferLayout.h"

#include "GLCommon.h"

namespace RGF {


	GLVertexArray::GLVertexArray() {
		GLCall(glGenVertexArrays(1, &m_RendererID));
	}
	GLVertexArray::~GLVertexArray() {
		GLCall(glDeleteVertexArrays(1, &m_RendererID));
	}

	void GLVertexArray::PushVertexBuffer(const std::shared_ptr<RGF::VertexBuffer>& buffer) {
		GLCall(glBindVertexArray(m_RendererID));
		buffer->Bind();
		
		m_Vbos.push_back(buffer);
	}

	void GLVertexArray::PushIndexBuffer(const std::shared_ptr<RGF::IndexBuffer>& buffer) {
		GLCall(glBindVertexArray(m_RendererID));
		buffer->Bind();

		m_Ibos.push_back(buffer);
	}

	void GLVertexArray::Bind() const {
		GLCall(glBindVertexArray(m_RendererID));
	}
	void GLVertexArray::Unbind() const {
		GLCall(glBindVertexArray(0));
	}


}