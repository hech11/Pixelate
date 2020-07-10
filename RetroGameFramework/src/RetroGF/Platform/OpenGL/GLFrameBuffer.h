#pragma once

#include "RetroGF/Rendering/API/FrameBuffer.h"


namespace RGF {

	class GLFrameBuffer : public FrameBuffer{



		public:
			GLFrameBuffer(const FrameBufferSpecs& specs);

			~GLFrameBuffer();

			void Bind() const override;
			void Unbind() const override;

			void Resize(unsigned int width, unsigned int height) override;

			unsigned int GetColorAttachment() const override { return m_ColorAttachment; }
			unsigned int GetRendererID() const override { return m_RendererID; }

			const FrameBufferSpecs& GetSpecs() const override { return m_Specs; }
		private :
			FrameBufferSpecs m_Specs;
			unsigned int MaxFrameBufferSize = 8192;
			unsigned int m_RendererID, m_ColorAttachment, m_DepthAttachment;

	};

}