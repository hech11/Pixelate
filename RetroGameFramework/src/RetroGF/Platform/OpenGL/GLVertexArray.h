#pragma once

#include "RetroGF/Rendering/API/VertexArray.h"
#include "RGFpch.h"

namespace RGF {

	class RGF_API GLVertexArray : public VertexArray {
		public :


			GLVertexArray();
			~GLVertexArray();

			void PushBuffer(RGF::VertexBuffer* buffer) override;

			void Bind() override;
			void Unbind() override;

			void Draw(unsigned int count) override;
		private :
			unsigned int m_RendererID;
			std::vector<VertexBuffer*> m_Buffers;
	};

}