#include "RGFpch.h"
#include "GLRenderer2D.h"

#include <GLAD/include/glad.h>
#include <GLM/glm/gtc/matrix_transform.hpp>

#include "RetroGF/Rendering/Sprite.h"

#include "GLCommon.h"

namespace RGF {
	void GLRenderer2D::SetDepthTesting(bool enable) {
		if (enable) {
			GLCall(glEnable(GL_DEPTH_TEST));
		} else {
			GLCall(glDisable(GL_DEPTH_TEST));
		}

	}
	void GLRenderer2D::SetStencilTesting(bool enable) {
		if (enable) {
			GLCall(glEnable(GL_STENCIL_TEST));
		} else {
			GLCall(glDisable(GL_STENCIL_TEST));
		}

	}
	void GLRenderer2D::RenderWireFrame(bool enable) {
		if (enable) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

	}



	void GLRenderer2D::ClearColor(float r, float g, float b) {
		GLCall(glClearColor(r, g, b, 1.0f));
	}
	void GLRenderer2D::ClearColor(unsigned char r, unsigned char g, unsigned char b) {
		GLCall(glClearColor(r / 256.0f, g / 256.0f, b / 256.0f, 1.0f));
	}

	void GLRenderer2D::Clear() {
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void GLRenderer2D::Submit(const Renderable* renderable) {
		m_RenderQueue.push_back(renderable);
	}
	void GLRenderer2D::Render() {
		while (!m_RenderQueue.empty()) {
			const auto& renderable = m_RenderQueue.front();

			renderable->GetVao()->Bind();
			renderable->GetIbo()->Bind();
			renderable->GetShader()->Bind();
			renderable->GetShader()->SetUniformMatrix("u_Model", glm::translate(glm::mat4(1.0f), renderable->GetPosition()));

			glDrawElements(GL_TRIANGLE_STRIP, renderable->GetIbo()->GetCount(), GL_UNSIGNED_SHORT, nullptr);

			
			renderable->GetVao()->Unbind();
			renderable->GetIbo()->Unbind();

			m_RenderQueue.pop_front();
		}

	}


	
}