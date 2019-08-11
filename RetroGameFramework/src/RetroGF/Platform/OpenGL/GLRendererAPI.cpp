#include "RGFpch.h"
#include "GLRendererAPI.h"

#include "GLCommon.h"

#include <GLAD\include\glad.h>

namespace RGF {



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


	void GLRendererAPI::Clear() {
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}
	void GLRendererAPI::SetClearColor(float r, float g, float b, float a) {
		GLCall(glClearColor(r, g, b, a));
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


	
	void* GLRendererAPI::MapBuffer(bool enable) {
		if (enable) {
			GLCall(return glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		} else {
			GLCall(glUnmapBuffer(GL_ARRAY_BUFFER));
			return nullptr;
		}
	}
	void GLRendererAPI::DrawElements(VertexArray* vao) {
		glDrawElements(GL_TRIANGLES, vao->GetIbos().GetCount(), vao->GetIbos().GetType(), nullptr);
	}



}