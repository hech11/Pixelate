#pragma once


#include "RetroGF/Rendering/API/Texture.h"
#include "RetroGF/Core.h"


// OpenGL's implemtation of the Texture class.



namespace RGF {

	class RGF_API GLTexture : public Texture {

		public:
			GLTexture(unsigned int width, unsigned int height, TextureParameters params);
			virtual ~GLTexture();

			void Bind(unsigned char slot = 0) const override;
			void Unbind() const override;

			void LoadTexture(const std::string& filepath) override;


			unsigned int GetHandleID() const override { return m_RendererID; };


		private :
			unsigned int m_RendererID;
	};
}