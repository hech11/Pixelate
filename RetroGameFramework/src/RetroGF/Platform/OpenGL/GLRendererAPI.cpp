#include "RGFpch.h"
#include "GLRendererAPI.h"

#include "GLCommon.h"

#include <GLAD\include\glad.h>
#include "RetroGF\Rendering\Renderer2D.h"

namespace RGF {

	static RenderAPICapabilities s_RenderCaps;

	unsigned int GLRendererAPI::ConvertBlendFunctions(const BlendFunc& func) {
		switch (func) {
		case GLRendererAPI::BlendFunc::ZERO:
			return GL_ZERO;
		case GLRendererAPI::BlendFunc::ONE:
			return GL_ONE;
		case GLRendererAPI::BlendFunc::SRC_COLOR:
			return GL_SRC_COLOR;
		case GLRendererAPI::BlendFunc::ONE_MINUS_SRC_COLOR:
			return GL_ONE_MINUS_SRC_COLOR;
		case GLRendererAPI::BlendFunc::SRC_ALPHA:
			return GL_SRC_ALPHA;
		case GLRendererAPI::BlendFunc::ONE_MINUS_SRC_ALPHA:
			return GL_ONE_MINUS_SRC_ALPHA;
		case GLRendererAPI::BlendFunc::DST_COLOR:
			return GL_DST_COLOR;
		case BlendFunc::DST_ALPHA:
			return GL_DST_ALPHA;
		case BlendFunc::ONE_MINUS_DST_COLOR:
			return GL_ONE_MINUS_DST_COLOR;
		case BlendFunc::CONSTANT_COLOR:
			return GL_CONSTANT_COLOR;
		case BlendFunc::ONE_MINUS_CONSTANT_COLOR:
			return GL_ONE_MINUS_CONSTANT_COLOR;
		case BlendFunc::CONSTANT_ALPHA:
			return GL_CONSTANT_ALPHA;
		case BlendFunc::ONE_MINUS_CONSTANT_ALPHA:
			return GL_ONE_MINUS_CONSTANT_ALPHA;
		}


	}
	void GLRendererAPI::Init() {


			s_RenderCaps.VendorName = (const char*)glGetString(GL_VENDOR);
			s_RenderCaps.RendererName = (const char*)glGetString(GL_RENDERER);
			s_RenderCaps.Version = (const char*)glGetString(GL_VERSION);
			s_RenderCaps.ContextName = "OpenGL";
			m_API = RendererAPI::API::OpenGL;

			glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &s_RenderCaps.MaxTextureSlots);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


			RGF_CORE_MSG("Rendering API : OpenGL\n");
			RGF_CORE_TRACE("----- Information -----\n");
			RGF_CORE_TRACE("Vendor: %s\n", s_RenderCaps.VendorName.c_str());
			RGF_CORE_TRACE("Version: %s\n", s_RenderCaps.Version.c_str());
			RGF_CORE_TRACE("Shading Language version: %s\n", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
			RGF_CORE_TRACE("GPU Card: %s\n", s_RenderCaps.RendererName.c_str());
			RGF_CORE_TRACE("Max texture slots: %d\n", s_RenderCaps.MaxTextureSlots);
			RGF_CORE_TRACE("--------------------------\n\n");


	}


	void GLRendererAPI::Clear() {
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}
	void GLRendererAPI::SetClearColor(float r, float g, float b, float a) {
		GLCall(glClearColor(r, g, b, a));
	}

	void GLRendererAPI::SetViewport(int x, int y, int width, int height) {
		GLCall(glViewport(x, y, width, height));
	}


	void GLRendererAPI::SetDepthTesting(bool enable) {
		if (enable) {
			GLCall(glEnable(GL_DEPTH_TEST));
		}
		else {
			GLCall(glDisable(GL_DEPTH_TEST));
		}
	}
	void GLRendererAPI::SetStencilTesting(bool enable){
		if (enable) {
			GLCall(glEnable(GL_STENCIL_TEST));
		}
		else {
			GLCall(glDisable(GL_STENCIL_TEST));
		}
	}
	void GLRendererAPI::RenderWireFrame(bool enable){
		if (enable) {
			GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		}
		else {
			GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		}
	}

	void GLRendererAPI::SetBlending(bool enable) {
		if (enable) {
			GLCall(glEnable(GL_BLEND));
		}
		else {
			GLCall(glDisable(GL_BLEND));
		}
	}

	void GLRendererAPI::SetBlendFunc(BlendFunc& source, BlendFunc& dest) {
		GLCall(glBlendFunc(ConvertBlendFunctions(source), ConvertBlendFunctions(dest)));

	}


	void GLRendererAPI::DrawElements(const Ref<VertexArray>& vao, unsigned int count) {


		unsigned int indexCount = count ? count : vao->GetIbos().GetCount();

		glDrawElements(GL_TRIANGLES, indexCount, vao->GetIbos().GetType(), nullptr);
	}

	const RenderAPICapabilities& GLRendererAPI::GetCaps() const {
		return s_RenderCaps;
	}


}