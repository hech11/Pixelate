#include "PXpch.h"
#include "GLTexture.h"

#include <glad/glad.h>
#include <STB/IMAGE/stb_image.h>

#include "GLCommon.h"

#include "Pixelate/Rendering/Renderer2D.h"

namespace Pixelate {
	static unsigned int GLConvertWrap(TextureProperties::Wrap wrap) {
		switch (wrap) {
			case TextureProperties::Wrap::Clamp_To_Edge:
				return GL_CLAMP_TO_EDGE;

			case TextureProperties::Wrap::Repeat:
				return GL_REPEAT;
		}
	}


	static unsigned int GLConvertFilter(TextureProperties::Filter filter) {
		switch (filter) {
		case TextureProperties::Filter::Nearest:
			return GL_NEAREST;

		case TextureProperties::Filter::Linear:
			return GL_LINEAR;
		}
	}

	static unsigned int GLConvertFormat(TextureProperties::Format format) {
		switch (format) {
		case TextureProperties::Format::RGB:
			return GL_RGB;

		case TextureProperties::Format::RGBA:
			return GL_RGBA;
		}
	}

	static unsigned int GLConvertFormatToBPP(TextureProperties::Format& format) {
		switch (format) {
			case TextureProperties::Format::RGB:
				return 3;

			case TextureProperties::Format::RGBA:
				return 4;
			}
	}

	GLTexture::GLTexture(const std::string& filepath, TextureProperties props) {

		props = m_Props;
		m_Props.FilePath = filepath;



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
			// This generates a pink texture if the texture could not be loaded. This code should be moved else where
			PX_ERROR("Failed to load '%s' !", filepath.c_str());
			unsigned int pinkCol = 0xFF00FF;
			m_Props.Width = 1;
			m_Props.Height = 1;
			m_Props.TexFormat = TextureProperties::Format::RGB;

			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GLConvertFormat(m_Props.TexFormat), m_Props.Width, m_Props.Height, 0, GLConvertFormat(m_Props.TexFormat), GL_UNSIGNED_BYTE, &pinkCol));
		}

		stbi_image_free(PixData);



	}
	GLTexture::GLTexture(unsigned int width, unsigned int height, TextureProperties::Format format) {
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

		GLCall(glActiveTexture(GL_TEXTURE0 + m_Props.Slot));
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
		PX_ASSERT(size == m_Props.Width * m_Props.Height * bpp, "Texture data must cover the entire texture!");
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Props.Width, m_Props.Height, GLConvertFormat(m_Props.TexFormat), GL_UNSIGNED_BYTE, data);

	}

	void GLTexture::SetData(const std::string& filepath) {

		unsigned char* PixData = stbi_load(filepath.c_str(), &m_Props.Width, &m_Props.Height, &m_Props.BPP, 4);

		if (PixData) {

			GLCall(glDeleteTextures(1, &m_RendererID));


			GLCall(glGenTextures(1, &m_RendererID));
			GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GLConvertWrap(m_Props.TexWrap)));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GLConvertWrap(m_Props.TexWrap)));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GLConvertFilter(m_Props.TexFilter)));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GLConvertFilter(m_Props.TexFilter)));


			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GLConvertFormat(m_Props.TexFormat), m_Props.Width, m_Props.Height, 0, GLConvertFormat(m_Props.TexFormat), GL_UNSIGNED_BYTE, PixData));

		}
		else {
			PX_ERROR("Failed to load '%s' !", filepath.c_str());
		}

		stbi_image_free(PixData);

	}

}