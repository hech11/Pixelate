#pragma once

#include "Pixelate/Rendering/API/Framebuffer.h"


namespace Pixelate {

	class GLFramebuffer : public Framebuffer {



		public:
			GLFramebuffer(const FramebufferSpecs& specs);

			~GLFramebuffer();

			void Bind() const override;
			void Unbind() const override;

			void Resize(uint32_t width, uint32_t height) override;

			void BindTexture(uint32_t attachmentID, uint32_t slot = 0) const override;

			void ClearColorAttachment(uint32_t attachment, int value) override;
			int ReadPixel(uint32_t colAttachment, int x, int y) override;


			unsigned int GetRendererID() const override { return m_RendererID; }

			const FramebufferSpecs& GetSpecs() const override { return m_Specs; }

			uint32_t GetColorAttachmentRenderID(uint32_t index) const override {
				int a = 12;
				int b = a + 12;
				return m_ColorAttachments[index]; 
			}
			uint32_t GetDepthAttachmentRenderID() const override { return m_DepthAttachment; }

			uint32_t GetWidth() const override { return m_Width; }
			uint32_t GetHeight() const override { return m_Height; }


			std::pair<uint32_t, uint32_t> GetDimensions() const override { return std::make_pair(m_Width, m_Height); }

		private:
			uint32_t m_RendererID;

			FramebufferSpecs m_Specs;
			uint32_t MaxFrameBufferSize = 8192;

			std::vector<uint32_t> m_ColorAttachments;
			uint32_t m_DepthAttachment;

			std::vector<FramebufferTextureSpecs> m_ColorTextureSpecs;
			FramebufferTextureSpecs m_DepthAttachmentSpecs = FramebufferTextureFormat::None;

			// might be useful to not alter the original width and height so we store a copy and modify that
			uint32_t m_Width, m_Height;
	};

}