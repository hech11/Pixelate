#pragma once


#include "RetroGF/Rendering/API/Buffer.h"

// OpenGL's implementation of Vertex and Index buffer


namespace RGF {

	class RGF_API GLVertexBuffer : public VertexBuffer {
		public :
			GLVertexBuffer(const void* data, unsigned int size);
			GLVertexBuffer(unsigned int size);


			virtual ~GLVertexBuffer();


			void SetData(const void* data, unsigned int size) override;
			void SetLayout(const BufferLayout& layout) override;


			const BufferLayout& GetLayout() const override { return m_Layout; }
			BufferLayout& GetLayout() override { return m_Layout; }

			void Bind() const override;
			void Unbind() const override;

			
		private :

			struct VertexBufferStatistics {
				unsigned int Size;
				BufferUsage Usage;
			};
			VertexBufferStatistics m_Stats;

			BufferLayout m_Layout;
			unsigned int m_RendererID;


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