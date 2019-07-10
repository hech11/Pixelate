#pragma once

#include "RetroGF/Rendering/API/VertexArray.h"
#include "RGFpch.h"



// OpenGL's implementation of 'VertexArray.h'

namespace RGF {

	class RGF_API GLVertexArray : public VertexArray {
		public :


			GLVertexArray();
			~GLVertexArray();


			void PushVertexBuffer(const std::shared_ptr<RGF::VertexBuffer>& buffer) override;
			void PushIndexBuffer(const std::shared_ptr<RGF::IndexBuffer>& buffer) override;


			virtual const std::vector<std::shared_ptr<RGF::VertexBuffer>>& GetVbos() const { return m_Vbos; }
			virtual const std::vector<std::shared_ptr<RGF::IndexBuffer>>& GetIbos() const { return m_Ibos; }


			void Bind() const override;
			void Unbind() const override;

		private :
			unsigned int m_RendererID;

			std::vector<std::shared_ptr<RGF::VertexBuffer>> m_Vbos;
			std::vector<std::shared_ptr<RGF::IndexBuffer>> m_Ibos;
	};

}