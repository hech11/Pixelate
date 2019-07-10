#include "RGFpch.h"
#include "GLVertexArray.h"

#include "GLCommon.h"

namespace RGF {


	static unsigned int BufferLayoutDataTypeToOpenGLType(BufferLayoutTypes type) 
	{
		switch(type) {
			case BufferLayoutTypes::Char2:	return GL_UNSIGNED_BYTE;
			case BufferLayoutTypes::Char3:	return GL_UNSIGNED_BYTE;
			case BufferLayoutTypes::Char4:	return GL_UNSIGNED_BYTE;
			case BufferLayoutTypes::Short2: return GL_UNSIGNED_SHORT;
			case BufferLayoutTypes::Short3: return GL_UNSIGNED_SHORT;
			case BufferLayoutTypes::Short4: return GL_UNSIGNED_SHORT;
			case BufferLayoutTypes::Int2:	return GL_UNSIGNED_INT;
			case BufferLayoutTypes::Int3:	return GL_UNSIGNED_INT;
			case BufferLayoutTypes::Int4:	return GL_UNSIGNED_INT;
			case BufferLayoutTypes::Float4: return GL_FLOAT;
			case BufferLayoutTypes::Mat4:	return GL_FLOAT_MAT4x3;
		}

		RGF_ASSERT(false, "No buffer types were listed!");
		return 0;
	}


	GLVertexArray::GLVertexArray() {
		GLCall(glGenVertexArrays(1, &m_RendererID));
	}
	GLVertexArray::~GLVertexArray() {
		GLCall(glDeleteVertexArrays(1, &m_RendererID));
	}

	void GLVertexArray::PushVertexBuffer(const std::shared_ptr<RGF::VertexBuffer>& buffer) {
		GLCall(glBindVertexArray(m_RendererID));
		buffer->Bind();
		

		uint32_t index = 0;
		const auto& layout = buffer->GetLayout();

		for (const auto& element : layout)
		{
			GLCall(glEnableVertexAttribArray(index));
			GLCall(glVertexAttribPointer(index, 
				element.count,
				BufferLayoutDataTypeToOpenGLType(element.type),
				element.normilized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.offset));


			index++;
		}


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