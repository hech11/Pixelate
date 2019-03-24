#include "RGFpch.h"
#include "GLRenderer2D.h"

#include "RetroGF/Rendering/Renderable.h"
#include <GLAD/include/glad.h>
namespace RGF {

	Renderer2D* Renderer2D::Create(const RenderingType& type = RenderingType::Batch) {
		return new GLRenderer2D(type);

	}


	void GLRenderer2D::Clear() {
		glClear(GL_COLOR_BUFFER_BIT);
	}


	void GLRenderer2D::Init() {

		m_Vao = new GLVertexArray;
		m_Vao->Bind();


		m_Vbo = new GLVertexBuffer(VertexBufferUsage::Dynamic);

		m_Vbo->Bind();
		m_Vbo->Resize(RENDERER_BUFFER_SIZE);

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(4);
		m_Vbo->SetLayout(layout);

		m_Vao->PushBuffer(m_Vbo);
		m_Vbo->Unbind();


		unsigned short indices[RENDERER_INDICIES_SIZE];
		int offset = 0;
		for (int i = 0; i < RENDERER_INDICIES_SIZE; i += 6) {
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
	void GLRenderer2D::ShutDown() {
		delete m_Vbo;
		delete m_Ibo;
		delete m_Vao;
	}


	void GLRenderer2D::Start() {
		m_Vbo->Bind();
		Buffer = (VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	}

	void GLRenderer2D::Submit(const Renderable* renderable) {

		const auto& Pos = renderable->GetPosition();
		const auto& Scale = renderable->GetScale();
		const auto& Color = renderable->GetColor();

		// 1st vertex
		Buffer->verticies = Pos;
		Buffer->color = Color;
		Buffer++;

		// 2st vertex
		Buffer->verticies = glm::vec3(Pos.x, Pos.y + Scale.y, Pos.z);
		Buffer->color = Color;
		Buffer++;

		// 3st vertex
		Buffer->verticies = glm::vec3(Pos.x + Scale.x, Pos.y + Scale.y, Pos.z);
		Buffer->color = Color;
		Buffer++;


		// 4st vertex
		Buffer->verticies = glm::vec3(Pos.x + Scale.x, Pos.y, Pos.z);
		Buffer->color = Color;
		Buffer++;

		m_IndexCount += 6;

	}
	void GLRenderer2D::Render() {
		m_Vao->Bind();
		m_Ibo->Bind();

		glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_SHORT, nullptr);

		m_Ibo->Unbind();
		m_Vao->Unbind();


		m_IndexCount = 0;

	}


	void GLRenderer2D::End() {

		glUnmapBuffer(GL_ARRAY_BUFFER);
		m_Vbo->Unbind();

	}
	
}
