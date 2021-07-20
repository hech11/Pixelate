#include "PXpch.h"
#include "GLFramebuffer.h"

#include "glad/glad.h"

#include "GLCommon.h"
#include "Pixelate/Rendering/Renderer2D.h"
#include "Pixelate/Core/Application.h"

namespace Pixelate {


	static void AttachColorAttachment(uint32_t id, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index) {

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, id, 0);
	}


	static void AttachDepthAttachment(uint32_t id, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height) {


		glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, id, 0);
	}



	GLFramebuffer::GLFramebuffer(const FramebufferSpecs& specs) 
		: m_Specs(specs), m_Width(m_Specs.Width), m_Height(m_Specs.Height), m_RendererID(0)
	{

		for (auto& attachment : m_Specs.Attachments.Attachments) {
			if (attachment.Format != FramebufferTextureFormat::Depth24Stencil8) {
				m_ColorTextureSpecs.emplace_back(attachment);
			}
			else {
				m_DepthAttachmentSpecs = attachment.Format;
			}
		}

		uint32_t width = m_Specs.Width;
		uint32_t height = m_Specs.Height;

		if (width == 0 || height == 0) {
			width = Application::GetApp().GetWindow().GetWidth();
			height = Application::GetApp().GetWindow().GetHeight();
		}
		Resize(width, height);
	}

	GLFramebuffer::~GLFramebuffer() {

		GLCall(glDeleteFramebuffers(1, &m_RendererID));
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		GLCall(glDeleteTextures(1, &m_DepthAttachment));

	}

	void GLFramebuffer::Bind() const {

		if (m_Width == 0 || m_Height == 0 || m_Width > MaxFrameBufferSize ||
			m_Height > MaxFrameBufferSize) {
			PX_CORE_WARN("Framebuffer attempted to resize to %fx%f!\n", m_Width, m_Height);
			return;
		}
		GLCall(glViewport(0, 0, m_Width, m_Height));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
	}

	void GLFramebuffer::Unbind() const {
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void GLFramebuffer::Resize(uint32_t  width, uint32_t height) {
		if (width == 0 || height == 0 || width > MaxFrameBufferSize || height > MaxFrameBufferSize) {
			PX_CORE_WARN("Framebuffer attempted to resize to %fx%f!\n", width, height);
			return;
		}

		m_Width = width;
		m_Height = height;

		if (m_RendererID) {
			GLCall(glDeleteFramebuffers(1, &m_RendererID));
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			GLCall(glDeleteTextures(1, &m_DepthAttachment));
		}
		

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);


		// color attachments
		// NOTE: Multisample?
		if (!m_ColorTextureSpecs.empty()) {
			m_ColorAttachments.resize(m_ColorTextureSpecs.size());
			glCreateTextures(GL_TEXTURE_2D, m_ColorAttachments.size(), m_ColorAttachments.data());

			for (uint32_t i = 0; i < m_ColorAttachments.size(); i++) {
				glBindTexture(GL_TEXTURE_2D, m_ColorAttachments[i]);
				switch (m_ColorTextureSpecs[i].Format)
				{
					case Pixelate::FramebufferTextureFormat::RGBA8:
						AttachColorAttachment(m_ColorAttachments[i], GL_RGBA8, GL_RGBA, width, height, i);
						break;
				}

			}


			// depth attachment
			if (m_DepthAttachmentSpecs.Format != FramebufferTextureFormat::None) {
				glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
				glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
				switch (m_DepthAttachmentSpecs.Format)
				{
					case Pixelate::FramebufferTextureFormat::RGBA8:
						AttachDepthAttachment(m_DepthAttachment, GL_DEPTH24_STENCIL8, GL_DEPTH_ATTACHMENT, width, height);
						break;
				}

			}
		}

		if (!m_ColorAttachments.empty()) {
			GLenum attachments[4] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachments.size(), attachments);
		}
		else {
			glDrawBuffer(GL_NONE); // this is a depth only pass
		}

		PX_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "The framebuffer is incomplete!\n");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


	}



	void GLFramebuffer::BindTexture(uint32_t attachmentID, uint32_t slot /*= 0*/) const
	{
		glBindTextureUnit(slot, m_ColorAttachments[attachmentID]);
	}



}