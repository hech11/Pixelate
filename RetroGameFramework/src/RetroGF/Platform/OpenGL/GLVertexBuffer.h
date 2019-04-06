#pragma once


#include "RetroGF/Rendering/API/VertexBuffer.h"

// OpenGL's implementation of 'VertexBuffer.h'


namespace RGF {

	class RGF_API GLVertexBuffer : public VertexBuffer {
		public :
			GLVertexBuffer(VertexBufferUsage usage);
			~GLVertexBuffer();


			void SetData(unsigned int size, const void* data) override;
			void Resize(unsigned int size) override;

			void SetLayout(const VertexBufferLayout& layout) override;


			void Bind() const override;
			void Unbind() const override;
			
		private :
			unsigned int m_RendererID;
			unsigned int m_Size;

			VertexBufferUsage m_Usage;
			VertexBufferLayout m_Layout;
	};

}