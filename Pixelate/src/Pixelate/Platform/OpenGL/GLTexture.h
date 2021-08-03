#pragma once


#include "Pixelate/Rendering/API/Texture.h"
#include "Pixelate/Core/Core.h"


// OpenGL's implementation of the Texture class.

namespace Pixelate {

	class PX_API GLTexture : public Texture {

		public:
			GLTexture(const std::string& filepath, TextureProperties props);
			GLTexture(unsigned int width, unsigned int height, TextureProperties::Format format);

			virtual ~GLTexture();

			uint32_t GetRendererID() const override { return m_RendererID; }


			void Bind(unsigned char slot = 0) const override;
			void Unbind() const override;

			void SetProperties(TextureProperties& props) { m_Props = props; }


			void SetData(void* data, unsigned int size) override;
			void SetData(const std::string& filepath) override;

			
			unsigned int GetHandleID() const override { return m_RendererID; };

			const TextureProperties& GetTextureProps() const override { return m_Props; }

			void Reload() override;


			bool operator==(const Texture& other) const override { return m_RendererID == other.GetHandleID(); };


		private :
			unsigned int m_RendererID;
	};
}