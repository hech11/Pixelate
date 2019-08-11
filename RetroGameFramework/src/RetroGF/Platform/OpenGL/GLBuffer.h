#pragma once


#include "RetroGF/Rendering/API/Buffer.h"

// OpenGL's implementation of Vertex and Index buffer


namespace RGF {

	class RGF_API GLVertexBuffer : public VertexBuffer {
		public :
			GLVertexBuffer(BufferUsage usage = BufferUsage::Static);
			virtual ~GLVertexBuffer();


			void SetData(const void* data) override;
			void Resize(unsigned int size) override;

			void SetLayout(const BufferLayout& layout) override;


			const BufferLayout& GetLayout() const override { return m_Layout; }
			BufferLayout& GetLayout() override { return m_Layout; }

			void Bind() const override;
			void Unbind() const override;

			
		private :
			unsigned int m_RendererID;

			unsigned int m_Size;
			BufferUsage m_Usage;
			BufferLayout m_Layout;

	};




	class RGF_API GLIndexBuffer : public IndexBuffer{
		public :

			GLIndexBuffer(unsigned int* data, unsigned int count);
			GLIndexBuffer(unsigned short* data, unsigned int count);
			GLIndexBuffer(unsigned char* data, unsigned int count);

			virtual ~GLIndexBuffer();



			unsigned int GetCount() const { return m_Count; };
			unsigned int GetType() const { return m_Type; };


			virtual void Bind() const override;
			virtual void Unbind() const override;


		private :
			unsigned int m_RendererID;

			unsigned int m_Count;
			unsigned int m_Type;
	};

}