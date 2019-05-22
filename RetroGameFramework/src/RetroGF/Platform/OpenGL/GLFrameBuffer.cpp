#include "RGFpch.h"
#include "GLFrameBuffer.h"
#include "GLTexture.h"

#include <GLAD/include/glad.h>


#include "GLCommon.h"

#include "RetroGF/Application.h"
namespace RGF {

	GLFrameBuffer::GLFrameBuffer(unsigned int width, unsigned int height) 
		: m_Width(width), m_Height(height) 
	{
		glGenFramebuffers(1, &m_Fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		glGenTextures(1, &m_TexCol);
		glBindTexture(GL_TEXTURE_2D, m_TexCol);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexCol, 0);

		glGenRenderbuffers(1, &m_Rbo);
		glBindFramebuffer(GL_RENDERBUFFER, m_Rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_RENDERBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_FRAMEBUFFER, m_Rbo);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			RGF_CORE_ERROR("Failed to create render buffer!");
		}


		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}
	GLFrameBuffer::~GLFrameBuffer() {
		glDeleteFramebuffers(1, &m_Fbo);
		glDeleteTextures(1, &m_TexCol);
		glDeleteRenderbuffers(1, &m_Rbo);
	}

	void GLFrameBuffer::Clear() const {
		GLCall(glClearColor(m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}


	void GLFrameBuffer::Bind() const {
		glBindTexture(GL_TEXTURE_2D, 0);
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo));
		GLCall(glViewport(0, 0, m_Width, m_Height));
	}
	void GLFrameBuffer::Unbind() const {
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		GLCall(glViewport(0, 0, Application::GetApp().GetWindow().GetWidth(), Application::GetApp().GetWindow().GetHeight()));

	}



}