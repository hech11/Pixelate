#include "RGFpch.h"
#include "GLRenderer2D.h"

#include <GLAD/include/glad.h>
#include <GLM/glm/gtc/matrix_transform.hpp>

#include "RetroGF/Rendering/Sprite.h"

#include "GLCommon.h"

namespace RGF {

	void GLRenderer2D::Clear() {
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
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

			glDrawElements(GL_TRIANGLES, renderable->GetIbo()->GetCount(), GL_UNSIGNED_SHORT, nullptr);
			renderable->GetVao()->Unbind();
			renderable->GetIbo()->Unbind();

			m_RenderQueue.pop_front();
		}

	}


	
}