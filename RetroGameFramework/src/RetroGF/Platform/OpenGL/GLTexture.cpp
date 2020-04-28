#include "RGFpch.h"
#include "GLTexture.h"

#include <GLAD/include/glad.h>
#include <STB/IMAGE/stb_image.h>

#include "GLCommon.h"

#include "RetroGF/Rendering/Renderer2D.h"

namespace RGF {
	static unsigned int GLConvertWrap(Texture::TextureProperties::Wrap wrap) {
		switch (wrap) {
			case Texture::TextureProperties::Wrap::Clamp_To_Edge:
				return GL_CLAMP_TO_EDGE;

			case Texture::TextureProperties::Wrap::Repeat:
				return GL_REPEAT;
		}
	}


	static unsigned int GLConvertFilter(Texture::TextureProperties::Filter filter) {
		switch (filter) {
		case Texture::TextureProperties::Filter::Nearest:
			return GL_NEAREST;

		case Texture::TextureProperties::Filter::Linear:
			return GL_LINEAR;
		}
	}

	static unsigned int GLConvertFormat(Texture::TextureProperties::Format format) {
		switch (format) {
		case Texture::TextureProperties::Format::RGB:
			return GL_RGB;

		case Texture::TextureProperties::Format::RGBA:
			return GL_RGBA;
		}
	}

	static unsigned int GLConvertFormatToBPP(Texture::TextureProperties::Format& format) {
		switch (format) {
			case Texture::TextureProperties::Format::RGB:
				return 3;

			case Texture::TextureProperties::Format::RGBA:
				return 4;
			}
	}

	GLTexture::GLTexture(const std::string& filepath, TextureProperties props) {

		props = m_Props;

		GLCall(glGenTextures(1, &m_RendererID));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GLConvertWrap(m_Props.TexWrap)));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GLConvertWrap(m_Props.TexWrap)));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GLConvertFilter(m_Props.TexFilter)));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GLConvertFilter(m_Props.TexFilter)));


		stbi_set_flip_vertically_on_load(true);
		unsigned char* PixData = stbi_load(filepath.c_str(), &m_Props.Width, &m_Props.Height, &m_Props.BPP, 4);
		if (PixData) {
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GLConvertFormat(m_Props.TexFormat), m_Props.Width, m_Props.Height, 0, GLConvertFormat(m_Props.TexFormat), GL_UNSIGNED_BYTE, PixData));
		}
		else {
			RGF_ERROR("Failed to load '%s' !", filepath.c_str());
		}

		stbi_image_free(PixData);


	}
	GLTexture::GLTexture(unsigned int width, unsigned int height, Texture::TextureProperties::Format format) {
		m_Props.Width = width;
		m_Props.Height = height;
		m_Props.TexFormat = format;

		GLCall(glGenTextures(1, &m_RendererID));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GLConvertWrap(m_Props.TexWrap)));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GLConvertWrap(m_Props.TexWrap)));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GLConvertFilter(m_Props.TexFilter)));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GLConvertFilter(m_Props.TexFilter)));


		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GLConvertFormat(m_Props.TexFormat), m_Props.Width, m_Props.Height, 0, GLConvertFormat(m_Props.TexFormat), GL_UNSIGNED_BYTE, 0));
		
	}


	GLTexture::~GLTexture() {
		GLCall(glDeleteTextures(1, &m_RendererID));
	}

	void GLTexture::Bind(unsigned char slot) const {
		
		m_Props.Slot = slot;
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));


		m_Props.IsBound = true;
	}
	void GLTexture::Unbind() const {
		
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		m_Props.IsBound = false;

	}

	void GLTexture::SetData(void* data, unsigned int size) {
		unsigned int bpp = GLConvertFormatToBPP(m_Props.TexFormat);
		RGF_ASSERT(size == m_Props.Width * m_Props.Height * bpp, "Texture data must cover the entire texture!");
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Props.Width, m_Props.Height, GLConvertFormat(m_Props.TexFormat), GL_UNSIGNED_BYTE, data);
	}

}