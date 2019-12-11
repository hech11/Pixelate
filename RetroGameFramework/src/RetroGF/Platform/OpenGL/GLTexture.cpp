#include "RGFpch.h"
#include "GLTexture.h"

#include <GLAD/include/glad.h>
#include <STB/IMAGE/stb_image.h>

#include "GLCommon.h"

#include "RetroGF/Rendering/Renderer2D.h"

namespace RGF {
	static unsigned int GLConvertWrap(TextureWrap wrap) {
		switch (wrap) {
			case TextureWrap::Clamp_To_Edge: 
				return GL_CLAMP_TO_EDGE;

			case TextureWrap::Repeat:
				return GL_REPEAT;
		}
	}


	static unsigned int GLConvertFilter(TextureFilter filter) {
		switch (filter) {
		case TextureFilter::Nearest:
			return GL_NEAREST;

		case TextureFilter::Linear:
			return GL_LINEAR;
		}
	}

	static unsigned int GLConvertFormat(TextureFormat format) {
		switch (format) {
		case TextureFormat::RGB:
			return GL_RGB;

		case TextureFormat::RGBA:
			return GL_RGBA;
		}
	}

	GLTexture::GLTexture(unsigned int width, unsigned int height, TextureParameters params) {
		m_Width = width;
		m_Height = height;
		m_Params = params;
		m_IsBound = false;

		GLCall(glGenTextures(1, &m_RendererID));
	}
	GLTexture::~GLTexture() {
		GLCall(glDeleteTextures(1, &m_RendererID));
	}

	void GLTexture::Bind(unsigned char slot) const {
		
		m_Slot = slot;
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));


		m_IsBound = true;
	}
	void GLTexture::Unbind() const {
		
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		m_IsBound = false;

	}

	void GLTexture::LoadTexture(const std::string& filepath) {


		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GLConvertWrap(m_Params.Wrap)));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GLConvertWrap(m_Params.Wrap)));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GLConvertFilter(m_Params.Filter)));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GLConvertFilter(m_Params.Filter)));


		stbi_set_flip_vertically_on_load(true);
		unsigned char* PixData = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_BPP, 4);
		if (PixData) {
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GLConvertFormat(m_Params.Format), m_Width, m_Height, 0, GLConvertFormat(m_Params.Format), GL_UNSIGNED_BYTE, PixData));
			GLCall(glGenerateMipmap(GL_TEXTURE_2D));
		} else {
			RGF_ERROR("Failed to load '%s' !", filepath.c_str());
		}

		stbi_image_free(PixData);


	}


}