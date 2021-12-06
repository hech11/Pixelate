#include "SandboxLayer.h"

#include <Pixelate/Rendering/API/VertexArray.h>
#include <Pixelate/Rendering/API/Buffer.h>
#include <Pixelate/Rendering/API/Shader/Shader.h>

#include <Pixelate/Rendering/RendererAPI.h>
#include <Pixelate/Rendering/RenderCommand.h>
#include <Pixelate/Editor/EditorCamera.h>

namespace Pixelate
{

	static Ref<VertexArray> vao;
	static Ref<VertexBuffer> vbo;
	static Ref<IndexBuffer> ibo;
	static Ref<Shader> shader;

	static Ref<EditorCamera> camera;
	static ViewportPanelProps props;

	static glm::vec2 winPos = glm::vec2(0.0f, 0.0f);
	static glm::vec2 winSize = glm::vec2(1280, 720);

	void SandboxLayer::Init()
	{

		float verts[] =
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
		};
		vao = VertexArray::Create();
		vbo = VertexBuffer::Create(verts, sizeof(verts));
		BufferLayout layout =
		{
			{ BufferLayoutTypes::Float3, "aPos"}
		};

		vbo->SetLayout(layout);
		vao->PushVertexBuffer(vbo);



		uint32_t indicies[] =
		{
			0, 1, 2, 2, 3, 0
		};
		ibo = IndexBuffer::Create(indicies, 6);

		vao->PushIndexBuffer(ibo);

		shader = Shader::Create("assets/Shaders/SimpleVulkan.shader");

		props.Size = &winSize;
		props.Position = &winPos;


		camera = CreateRef<EditorCamera>(16.0f / 9.0f, props);

	}

	void SandboxLayer::ShutDown()
	{

	}

	void SandboxLayer::OnUpdate(float dt)
	{
		camera->OnUpdate(dt);


		RenderCommand::Clear();

		vao->Bind();
		ibo->Bind();
		shader->Bind();
		shader->SetUniformMatrix("u_ViewProj", camera->GetViewProjectionMatrix());

		RenderCommand::DrawElements(vao, PimitiveRenderType::Triangles, ibo->GetCount());

	}

	void SandboxLayer::OnEvent(Pixelate::Event& e)
	{
		camera->OnEvent(e);

	}

	void SandboxLayer::OnImguiRender()
	{

	}

}
