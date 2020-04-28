#include "RGFpch.h"
#include "GLVertexArray.h"

#include "GLCommon.h"

namespace RGF {


	static unsigned int BufferLayoutDataTypeToOpenGLType(BufferLayoutTypes type) 
	{
		switch(type) {
			case BufferLayoutTypes::UChar:	return GL_UNSIGNED_BYTE;
			case BufferLayoutTypes::UChar2:	return GL_UNSIGNED_BYTE;
			case BufferLayoutTypes::UChar3:	return GL_UNSIGNED_BYTE;
			case BufferLayoutTypes::UChar4:	return GL_UNSIGNED_BYTE;
			case BufferLayoutTypes::Char:	return GL_BYTE;
			case BufferLayoutTypes::Char2:	return GL_BYTE;
			case BufferLayoutTypes::Char3:	return GL_BYTE;
			case BufferLayoutTypes::Char4:	return GL_BYTE;
			case BufferLayoutTypes::Short2: return GL_SHORT;
			case BufferLayoutTypes::Short3: return GL_SHORT;
			case BufferLayoutTypes::Short4: return GL_SHORT;
			case BufferLayoutTypes::Int:	return GL_INT;
			case BufferLayoutTypes::Int2:	return GL_INT;
			case BufferLayoutTypes::Int3:	return GL_INT;
			case BufferLayoutTypes::Int4:	return GL_INT;
			case BufferLayoutTypes::Float4: return GL_FLOAT;
			case BufferLayoutTypes::Float3: return GL_FLOAT;
			case BufferLayoutTypes::Float2: return GL_FLOAT;
			case BufferLayoutTypes::Float : return GL_FLOAT;
			case BufferLayoutTypes::Mat4:	return GL_FLOAT;
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

	void GLVertexArray::PushVertexBuffer(const Ref<RGF::VertexBuffer>& buffer) {
		RGF_ASSERT(buffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		GLCall(glBindVertexArray(m_RendererID));
		buffer->Bind();
		

		unsigned int index = 0;
		const auto& layout = buffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				BufferLayoutDataTypeToOpenGLType(element.type),
				(element.normilized ? GL_TRUE : GL_FALSE),
				layout.GetStride(),
				(const void*)element.offset);
			index++;
		}

		m_Vbos.push_back(buffer);
	}


	void GLVertexArray::PushIndexBuffer(const Ref<RGF::IndexBuffer>& buffer) {
		GLCall(glBindVertexArray(m_RendererID));
		buffer->Bind();


		m_Ibo = buffer;
	}

	void GLVertexArray::Bind() const {
		GLCall(glBindVertexArray(m_RendererID));
	}
	void GLVertexArray::Unbind() const {
		GLCall(glBindVertexArray(0));
	}


}