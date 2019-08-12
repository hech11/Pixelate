#pragma once

#include "RetroGF/Rendering/API/VertexArray.h"
#include "RGFpch.h"



// OpenGL's implementation of 'VertexArray.h'

namespace RGF {

	class RGF_API GLVertexArray : public VertexArray {
		public :


			GLVertexArray();
			~GLVertexArray();


			void PushVertexBuffer(const Ref<RGF::VertexBuffer>& buffer) override;
			void PushIndexBuffer(const Ref<RGF::IndexBuffer>& buffer) override;


			virtual const std::vector<Ref<RGF::VertexBuffer>>& GetVbos() const override { return m_Vbos; }
			virtual const RGF::IndexBuffer& GetIbos() const override { return *m_Ibo; }


			void Bind() const override;
			void Unbind() const override;

		private :
			unsigned int m_RendererID;

			std::vector<Ref<RGF::VertexBuffer>> m_Vbos;
			Ref<RGF::IndexBuffer> m_Ibo;
	};

}