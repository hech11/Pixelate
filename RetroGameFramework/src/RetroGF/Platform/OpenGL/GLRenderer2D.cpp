#include "RGFpch.h"
#include "GLRenderer2D.h"

#include <GLAD/include/glad.h>
#include <GLM/glm/gtc/matrix_transform.hpp>

#include "RetroGF/Rendering/Sprite.h"

#include "GLCommon.h"


// Has implementations of both GLRenderer2D and GLBatchRenderer2D.

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







	void GLBatchRenderer2D::SetDepthTesting(bool enable) {
		if (enable) {
			GLCall(glEnable(GL_DEPTH_TEST));
		} else {
			GLCall(glDisable(GL_DEPTH_TEST));
		}

	}
	void GLBatchRenderer2D::SetStencilTesting(bool enable) {
		if (enable) {
			GLCall(glEnable(GL_STENCIL_TEST));
		} else {
			GLCall(glDisable(GL_STENCIL_TEST));
		}

	}
	
	void GLBatchRenderer2D::RenderWireFrame(bool enable) {
		if (enable) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

	}



	void GLBatchRenderer2D::ClearColor(float r, float g, float b) {
		GLCall(glClearColor(r, g, b, 1.0f));
	}
	void GLBatchRenderer2D::ClearColor(unsigned char r, unsigned char g, unsigned char b) {
		GLCall(glClearColor(r / 256.0f, g / 256.0f, b / 256.0f, 1.0f));
	}


	void GLBatchRenderer2D::Clear() {
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}


	void GLBatchRenderer2D::Init() {
		m_Vao = new GLVertexArray;
		m_Vao->Bind();
		
		m_Vbo = new GLVertexBuffer(VertexBufferUsage::Dynamic);

		m_Vbo->Bind();
		m_Vbo->Resize(RENDERER_BUFFER_SIZE);

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<unsigned char>(4);
		m_Vbo->SetLayout(layout);

		m_Vao->PushBuffer(m_Vbo);
		m_Vbo->Unbind();


		unsigned short indices[RENDERER_INDICIES_SIZE];
		unsigned short offset = 0;
		for (unsigned short i = 0; i < RENDERER_INDICIES_SIZE; i += 6) {
			indices[i] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;
			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		m_Ibo = new GLIndexBuffer(indices, RENDERER_INDICIES_SIZE);
		m_Vao->Unbind();
	}
	void GLBatchRenderer2D::ShutDown() {
		delete m_Vbo;
		delete m_Ibo;
		delete m_Vao;
	}


	void GLBatchRenderer2D::Start() {
		m_Vbo->Bind();
		GLCall(Buffer = (VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	}

	void GLBatchRenderer2D::Submit(const Renderable* renderable) {

		const auto& Pos = renderable->GetPosition();
		const auto& Scale = renderable->GetScale();
		const auto& Color = renderable->GetColor();

		unsigned short r = Color.x * 255.0f;
		unsigned short g = Color.y * 255.0f;
		unsigned short b = Color.z * 255.0f;
		unsigned short a = Color.w * 255.0f;

		unsigned int c = a << 24 | b << 16 | g << 8 | r;
		// 1st vertex
		Buffer->verticies = Pos;
		Buffer->color = c;
		Buffer++;

		// 2st vertex
		Buffer->verticies = { Pos.x, Pos.y + Scale.y, Pos.z };
		Buffer->color = c;
		Buffer++;

		// 3st vertex
		Buffer->verticies = { Pos.x + Scale.x, Pos.y + Scale.y, Pos.z };
		Buffer->color = c;
		Buffer++;


		// 4st vertex
		Buffer->verticies = { Pos.x + Scale.x, Pos.y, Pos.z };
		Buffer->color = c;
		Buffer++;

		m_IndexCount += 4;

	}
	void GLBatchRenderer2D::Render() {
		m_Vao->Bind();
		m_Ibo->Bind();

		GLCall(glDrawElements(GL_TRIANGLE_STRIP, m_IndexCount, GL_UNSIGNED_SHORT, nullptr));


		m_IndexCount = 0;

	}


	void GLBatchRenderer2D::End() {

		GLCall(glUnmapBuffer(GL_ARRAY_BUFFER));
		m_Vbo->Unbind();

	}

	
}