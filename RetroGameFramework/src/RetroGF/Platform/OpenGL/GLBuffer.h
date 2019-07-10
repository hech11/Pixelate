#pragma once


#include "RetroGF/Rendering/API/Buffer.h"

// OpenGL's implementation of Vertex and Index buffer


namespace RGF {

	class RGF_API GLVertexBuffer : public VertexBuffer {
		public :
			GLVertexBuffer(VertexBufferUsage usage);
			virtual ~GLVertexBuffer();


			virtual void SetData(unsigned int size, const void* data) override;
			virtual void Resize(unsigned int size) override;
			virtual void SetLayout(const VertexBufferLayout& layout) override;


			virtual void Bind() const override;
			virtual void Unbind() const override;
			
		private :
			unsigned int m_RendererID;
			unsigned int m_Size;

			VertexBufferUsage m_Usage;
			VertexBufferLayout m_Layout;
	};




	class RGF_API GLIndexBuffer : public IndexBuffer {
		public :

			GLIndexBuffer(unsigned int* data, unsigned int count);
			GLIndexBuffer(unsigned short* data, unsigned int count);
			GLIndexBuffer(unsigned char* data, unsigned int count);

			virtual ~GLIndexBuffer();

			virtual void Bind() const override;
			virtual void Unbind() const override;


			unsigned int GetCount() const { return m_Count; }
		private :
			unsigned int m_RendererID;
			unsigned int m_Count;
	};

}