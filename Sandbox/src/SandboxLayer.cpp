#include "SandboxLayer.h"

#include <Pixelate/Rendering/API/VertexArray.h>
#include <Pixelate/Rendering/API/Buffer.h>
#include <Pixelate/Rendering/API/Shader/Shader.h>

#include <Pixelate/Rendering/RendererAPI.h>
#include <Pixelate/Rendering/RenderCommand.h>
#include <Pixelate/Editor/EditorCamera.h>
#include <Pixelate/Rendering/API/Texture.h>
#include "Pixelate/Rendering/Renderer2D.h"


// cross compiling shaders with SPIR-V + shaderc

namespace Pixelate
{

	static Ref<VertexArray> vao;
	static Ref<VertexBuffer> vbo;
	static Ref<IndexBuffer> ibo;
	static Ref<Shader> shader;
	static Ref<Texture> texture;

	static Ref<UniformBuffer> cameraBuffer;

	static Ref<EditorCamera> camera;
	static ViewportPanelProps props;

	static glm::vec2 winPos = glm::vec2(0.0f, 0.0f);
	static glm::vec2 winSize = glm::vec2(1280, 720);

	struct CameraData
	{
		glm::mat4 ViewProj;
	};

	static CameraData camData;

	void SandboxLayer::Init()
	{

		float verts[] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		vao = VertexArray::Create();
		vbo = VertexBuffer::Create(verts, sizeof(verts));
		BufferLayout layout =
		{
			{ BufferLayoutTypes::Float3, "aPos"},
			{ BufferLayoutTypes::Float2, "aTexCoord"}
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
		texture = Texture::Create("assets/graphics/sprite.png");
		texture->Bind();

		props.Size = &winSize;
		props.Position = &winPos;


		camera = CreateRef<EditorCamera>(16.0f / 9.0f, props);
		cameraBuffer = UniformBuffer::Create(sizeof(CameraData), 0);
	}

	void SandboxLayer::ShutDown()
	{

	}

	void SandboxLayer::OnUpdate(float dt)
	{
		camera->OnUpdate(dt);
		RenderCommand::Clear();
	
#if 1
		vao->Bind();
		ibo->Bind();
		shader->Bind();
		camData.ViewProj = camera->GetViewProjectionMatrix();
		cameraBuffer->SetData(&camData, sizeof(CameraData), 0);
		RenderCommand::DrawElements(vao, PimitiveRenderType::Triangles, ibo->GetCount());
#else
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 0.0f });

		Renderer2D::BeginScene(camera.get());
		Renderer2D::DrawSceneGrid(camera->GetOrthographicSize());


		Renderer2D::DrawSprite(transform, texture, {1.0f, 1.0f, 1.0f, 1.0f}, 1);
		Renderer2D::EndScene();

#endif

	}

	void SandboxLayer::OnEvent(Pixelate::Event& e)
	{
		camera->OnEvent(e);

	}

	void SandboxLayer::OnImguiRender()
	{

	}

}
