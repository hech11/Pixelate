#include "PXpch.h"
#include "SceneRenderer.h"

#include "Pixelate/Rendering/TextureManager.h"


namespace Pixelate
{



	Ref<Scene> SceneRenderer::s_CurrentScene;
	DrawList SceneRenderer::s_GeoDrawList;
	DrawList SceneRenderer::s_DebugDrawList;
	DrawList SceneRenderer::s_PhysicsDebugDrawList;

	SceneRenderer::CameraData SceneRenderer::s_CameraBufferData;
	SceneRenderer::GridData SceneRenderer::s_GridBufferData;

	Ref<UniformBuffer> SceneRenderer::CameraUniformBuffer;
	Ref<UniformBuffer> SceneRenderer::GridUniformBuffer;

	Ref<Pixelate::RenderPass> SceneRenderer::s_CompositePass;

	Ref<Pixelate::RenderPass> SceneRenderer::s_GeoRenderPass;
	Ref<Pixelate::RenderPass> SceneRenderer::s_SceneGridPass;
	Ref<Pixelate::RenderPass> SceneRenderer::s_DebuggingOutlinePass;


	SceneRenderer::SceneRenderer(Ref<Scene>& scene) 
	{
		s_CurrentScene = scene;
		Init();
	}

	void SceneRenderer::Init()
	{


		auto& shaderLibrary = Renderer2D::GetShaderLibrary();

		shaderLibrary.Load("DefaultTexturedShader", "Shaders/DefaultTexturedShader.pxShader");
		shaderLibrary.Load("GridShader", "Shaders/SceneGrid.pxShader");
		shaderLibrary.LoadExternalResource("FinalSceneComposite", "resources/shaders/FinalSceneComposite.pxShader");

		CameraUniformBuffer = UniformBuffer::Create(sizeof(CameraData), 0);

		s_CompositePass = CreateRef<RenderPass>();
		s_CompositePass->DebugName = "CompositePass";
		s_CompositePass->Shader = shaderLibrary.Get()["FinalSceneComposite"]; 
		
		FramebufferSpecs specs;
		specs.Attachments = { FramebufferTextureFormat::RGBA8 };
		specs.Width = 960;
		specs.Height = 540;

		s_CompositePass->FrameBufferTarget = Framebuffer::Create(specs);
		FramebufferPool::Add(s_CompositePass->FrameBufferTarget);

		CreateSceneGridPass();
		CreateGeoPass();
		CreateDebugOutlinePass();



	}

	void SceneRenderer::SetScene(const Ref<Scene>& scene)
	{
		s_CurrentScene = scene;
	}


	void SceneRenderer::SubmitSprite(const TransformComponent& transform, const SpriteRendererComponent& sprite, int entityID)
	{

		auto& shader = sprite.Shader;
		if (s_GeoDrawList.find(shader) == s_GeoDrawList.end())
		{
			s_GeoDrawList[shader] = CreateRef<DrawData>();
			s_GeoDrawList[shader]->Invalidate();
		}

		Renderer2D::BeginRenderPass(s_GeoRenderPass, s_GeoDrawList);
		Renderer2D::DrawSpriteWithShader(transform, sprite, entityID);

	}

	void SceneRenderer::SubmitLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4 & color)
	{

		auto& shader = Renderer2D::GetShaderLibrary().Get()["DefaultTexturedShader"];
		if (s_GeoDrawList.find(shader) == s_GeoDrawList.end())
		{
			s_GeoDrawList[shader] = CreateRef<DrawData>();
			s_GeoDrawList[shader]->Invalidate();
		}

		Renderer2D::DrawLine(p0, p1, color);
	}

	void SceneRenderer::BeginScene(const glm::mat4& viewProj)
	{
		s_CameraBufferData.ViewProj = viewProj;
		TextureManager::GetManagerData().TextureSlotIndex = 1;

		for (auto& [shader, command] : s_GeoDrawList)
		{
			command->IndexCount = 0;
			command->PtrData = command->PtrBase;
		}
		for (auto& [shader, command] : s_PhysicsDebugDrawList)
		{
			command->IndexCount = 0;
			command->PtrData = command->PtrBase;
		}
		for (auto& [shader, command] : s_DebugDrawList)
		{
			command->IndexCount = 0;
			command->PtrData = command->PtrBase;
		}

		Renderer2D::BeginRenderPass(s_GeoRenderPass, s_GeoDrawList);
	}

	void SceneRenderer::EndScene()
	{

		FlushDrawLists();
	}

	void SceneRenderer::OnImguiRender()
	{

	}

	void SceneRenderer::DrawSceneGrid(float gridAlpha)
	{

// 		s_ShaderLibrary.Get()["GridShader"]->Bind();
// 
// 		SceneData->GridBufferData.ViewProj = SceneData->CameraBuffer.ViewProj;
// 		SceneData->GridBufferData.Zoom = gridAlpha;
// 		SceneData->GridUniformBufferData->SetData(&SceneData->GridBufferData, sizeof(GridData), 0);
// 
// 
// 		SceneData->SceneGridVertexArray->Bind();
// 		SceneData->SceneGridVertexArray->GetIbos().Bind();
// 
// 		RenderCommand::DrawElements(SceneData->SceneGridVertexArray, PimitiveRenderType::Triangles, SceneData->SceneGridVertexArray->GetIbos().GetCount());
// 
// 		s_ShaderLibrary.Get()["GridShader"]->Unbind();
// 		SceneData->SceneGridVertexArray->Unbind();
// 		SceneData->SceneGridVertexArray->GetIbos().Unbind();

	}

	void SceneRenderer::CreateGeoPass()
	{
		auto& renderPassPool = Renderer2D::GetRenderPassPool();
		auto& shaderLibrary = Renderer2D::GetShaderLibrary();

		Ref<Shader> DefaultTexturedShader = shaderLibrary.Get()["DefaultTexturedShader"];

		s_GeoDrawList[DefaultTexturedShader] = CreateRef<DrawData>();
		 auto& drawData = s_GeoDrawList[DefaultTexturedShader];
		drawData->Invalidate();

		s_GeoRenderPass = CreateRef<RenderPass>();
		s_GeoRenderPass->DebugName = "GeoPass";
		s_GeoRenderPass->Shader = DefaultTexturedShader;

		FramebufferSpecs specs;
		specs.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };
		specs.Width = 960;
		specs.Height = 540;

		s_GeoRenderPass->FrameBufferTarget = Framebuffer::Create(specs);
		FramebufferPool::Add(s_GeoRenderPass->FrameBufferTarget);



		renderPassPool.GenerateDrawList(s_GeoRenderPass, s_GeoDrawList);
	}

	void SceneRenderer::CreateSceneGridPass()
	{

	}

	void SceneRenderer::CreateDebugOutlinePass()
	{

	}

	void SceneRenderer::GeoPass()
	{
		Renderer2D::BeginRenderPass(s_GeoRenderPass, s_GeoDrawList);

		CameraUniformBuffer->SetData(&s_CameraBufferData, sizeof(CameraData), 0);

		for (auto& [shader, command] : s_GeoDrawList)
 		{
 			shader->Bind();
 
 			uint32_t size = (uint8_t*)command->PtrData - (uint8_t*)command->PtrBase;
 
 			if (size)
 			{
 				command->Vao->Bind();
 				command->Vao->GetIbos().Bind();
 				command->Vbo->SetData(command->PtrBase, size);
 
 				TextureManager::BindAllTextures();
 
 				RenderCommand::DrawElements(command->Vao, PimitiveRenderType::Triangles, command->IndexCount);
 
 			}
 		}


		Renderer2D::EndRenderPass(s_GeoRenderPass);

	}

	void SceneRenderer::SceneGridPass()
	{

	}

	void SceneRenderer::DebugOutlinePass()
	{

	}

	void SceneRenderer::FlushDrawLists()
	{

		SceneGridPass();

		RenderCommand::Clear();
		GeoPass();

		DebugOutlinePass();

	}

}