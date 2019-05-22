#pragma once

#include "RetroGF/Rendering/API/FrameBuffer.h"


namespace RGF {

	class GLFrameBuffer : public FrameBuffer {
		public :

			GLFrameBuffer(unsigned int width, unsigned int height);
			~GLFrameBuffer();

			void Clear() const override;

			void Bind() const override;
			void Unbind() const override;



			inline unsigned int GetWidth() const override { return m_Width; }
			inline unsigned int GetHeight() const override { return m_Height; }

			inline unsigned int GetTexture() const override { return m_TexCol; }
			inline void SetClearColor(const glm::vec4& color) override { m_ClearColor = color; }

		private :
			unsigned int m_Fbo, m_TexCol, m_Rbo, m_Width, m_Height;
			glm::vec4 m_ClearColor;
	};

}