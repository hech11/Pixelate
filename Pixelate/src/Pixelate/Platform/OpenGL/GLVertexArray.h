#pragma once

#include "Pixelate/Rendering/API/VertexArray.h"
#include "PXpch.h"



// OpenGL's implementation of 'VertexArray.h'

namespace Pixelate {

	class PX_API GLVertexArray : public VertexArray {
		public :


			GLVertexArray();
			~GLVertexArray();

			
			void PushVertexBuffer(const Ref<Pixelate::VertexBuffer>& buffer) override;
			void PushIndexBuffer(const Ref<Pixelate::IndexBuffer>& buffer) override;


			virtual const std::vector<Ref<Pixelate::VertexBuffer>>& GetVbos() const override { return m_Vbos; }
			virtual const Pixelate::IndexBuffer& GetIbos() const override { return *m_Ibo; }


			void Bind() const override;
			void Unbind() const override;

		private :
			unsigned int m_RendererID;

			std::vector<Ref<Pixelate::VertexBuffer>> m_Vbos;
			Ref<Pixelate::IndexBuffer> m_Ibo;
	};

}