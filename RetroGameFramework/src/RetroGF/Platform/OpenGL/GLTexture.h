#pragma once


#include "RetroGF/Rendering/API/Texture.h"
#include "RetroGF/Core/Core.h"


// OpenGL's implementation of the Texture class.

namespace RGF {

	class RGF_API GLTexture : public Texture {

		public:
			GLTexture(const std::string& filepath, TextureProperties props);
			GLTexture(unsigned int width, unsigned int height, Texture::TextureProperties::Format format);

			virtual ~GLTexture();

			void Bind(unsigned char slot = 0) const override;
			void Unbind() const override;

			void SetData(void* data, unsigned int size) override;

			
			unsigned int GetHandleID() const override { return m_RendererID; };


			bool operator==(const Texture& other) const override { return m_RendererID == other.GetHandleID(); };


		private :
			unsigned int m_RendererID;
	};
}