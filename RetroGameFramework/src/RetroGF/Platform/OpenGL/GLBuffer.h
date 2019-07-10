#pragma once


#include "RetroGF/Rendering/API/Buffer.h"

// OpenGL's implementation of Vertex and Index buffer


namespace RGF {

	class RGF_API GLVertexBuffer : public VertexBuffer {
		public :
			GLVertexBuffer(unsigned int size, const void* data, BufferUsage usage = BufferUsage::Static);
			~GLVertexBuffer();


			void SetData(const void* data) override;
			void Resize(unsigned int size) override;

			void SetLayout(const BufferLayout& layout) override;


			void Bind() const override;
			void Unbind() const override;

			
		private :
			unsigned int m_RendererID;

	};




	class RGF_API GLIndexBuffer : public IndexBuffer {
		public :

			GLIndexBuffer(unsigned int* data, unsigned int count);
			GLIndexBuffer(unsigned short* data, unsigned int count);
			GLIndexBuffer(unsigned char* data, unsigned int count);

			~GLIndexBuffer();

			virtual void Bind() const override;
			virtual void Unbind() const override;


		private :
			unsigned int m_RendererID;
	};

}