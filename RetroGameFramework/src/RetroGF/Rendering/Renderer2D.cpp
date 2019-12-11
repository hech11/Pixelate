#include "RGFpch.h"
#include "Renderer2D.h"


#include "RetroGF/Rendering/RenderingContext.h"
#include "RetroGF/Rendering/RenderCommand.h"

#include "RetroGF/Debug/Instrumentor.h"

namespace RGF {

	Renderer2D::SceneData* Renderer2D::m_SceneData = nullptr;


	VertexData* Renderer2D::Buffer = nullptr;
	unsigned int Renderer2D::m_IndexCount = 0;

	Ref<VertexArray> Renderer2D::m_Vao = nullptr;
	Ref<VertexBuffer> Renderer2D::m_Vbo = nullptr;
	Ref<IndexBuffer> Renderer2D::m_Ibo = nullptr;

	
	Scoped<MaterialManager> Renderer2D::s_MatManager = std::make_unique<MaterialManager>();
	Scoped<ShaderManager> Renderer2D::s_ShaderManager = std::make_unique<ShaderManager>();
	Scoped<TextureManager> Renderer2D::s_TextureManager = std::make_unique<TextureManager>();


	void Renderer2D::Init() {
		RGF_PROFILE_FUNCTION();

		m_SceneData = new SceneData;

		{
			RGF_PROFILE_SCOPE("Renderer2D::Init::Setting-VertexBuffer");

			m_Vao = VertexArray::Create();
			m_Vao->Bind();
		
			m_Vbo = VertexBuffer::Create(BufferUsage::Dynamic);
			m_Vbo->Bind();
			m_Vbo->Resize(RENDERER_BUFFER_SIZE);
			m_Vbo->SetData(nullptr);



		BufferLayout layout = 
		{
			{ BufferLayoutTypes::Float3, "aPos"},
			{ BufferLayoutTypes::Char4, "aColor", true},
			{ BufferLayoutTypes::Float2, "aTexCoords"},

		};




		m_Vbo->SetLayout(layout);
		
		m_Vao->PushVertexBuffer(m_Vbo);
		m_Vbo->Unbind();

		}

		{

			RGF_PROFILE_SCOPE("Renderer2D::Init::Setting-IndexBuffer");

			unsigned short indices[RENDERER_INDICIES_SIZE];
			RGF_CORE_WARN("Indicies are unsigned shorts, This may cause problems. Change to unsigned ints if so.\n");
			int offset = 0;

			for (unsigned int i = 0; i < RENDERER_INDICIES_SIZE; i += 6) {
				indices[i] = offset + 0;
				indices[i + 1] = offset + 1;
				indices[i + 2] = offset + 2;
				indices[i + 3] = offset + 2;

				indices[i + 4] = offset + 3;
				indices[i + 5] = offset + 0;

				offset += 4;
			}

			m_Ibo = IndexBuffer::Create(indices, RENDERER_INDICIES_SIZE);
			m_Vao->PushIndexBuffer(m_Ibo);
			m_Vao->Unbind();

		}

	}

	void Renderer2D::Start(RGF::OrthographicCamera* camera) {

		m_SceneData->ViewProjectionMatrix = camera->GetViewProjectionMatrix();

		m_Vbo->Bind();
		Buffer = (VertexData*)RenderCommand::MapBuffer(true);
	}
	void Renderer2D::End() {

		RenderCommand::MapBuffer(false);
		m_Vbo->Unbind();
	}

	void Renderer2D::Submit(const Ref<Renderable>& renderable) {

		const auto& Pos = renderable->GetPosition();
		const auto& Scale = renderable->GetScale();
		const auto& Color = renderable->GetColor();
		const auto& Uv = renderable->GetUV();


		unsigned char r = Color.x * 255.0f;
		unsigned char g = Color.y * 255.0f;
		unsigned char b = Color.z * 255.0f;
		unsigned char a = Color.w * 255.0f;

		unsigned int c = a << 24 | b << 16 | g << 8 | r;



		// 1st vertex -- bottom left
		Buffer->verticies = Pos;
		Buffer->color = c;
		Buffer->uv = Uv[0];
		Buffer++;

		// 2st vertex -- bottom right
		Buffer->verticies = { Pos.x + Scale.x, Pos.y, Pos.z };
		Buffer->color = c;
		Buffer->uv = Uv[1];
		Buffer++;

		// 3st vertex -- top right
		Buffer->verticies = { Pos.x + Scale.x, Pos.y + Scale.y, Pos.z };
		Buffer->color = c;
		Buffer->uv = Uv[2];
		Buffer++;


		// 4st vertex -- top left
		Buffer->verticies = { Pos.x, Pos.y + Scale.y, Pos.z };
		Buffer->color = c;
		Buffer->uv = Uv[3];
		Buffer++;

		m_IndexCount += 6;

	}


	void Renderer2D::Render() {

		m_Vao->Bind();
		m_Ibo->Bind();

		RenderCommand::DrawElements(m_Vao);

		m_IndexCount = 0;
	}



	
}