#include "PXpch.h"
#include "SceneRenderer.h"

#include "Pixelate/Rendering/TextureManager.h"


namespace Pixelate
{


	SceneRenderer::SceneRenderer() 
	{
		Init();
	}

	void SceneRenderer::Init()
	{

		auto& shaderLibrary = Renderer2D::GetShaderLibrary();

		// TODO : Should be able to write "assets/Shaders/...." instead of "Shaders/.."
		shaderLibrary.Load("DefaultTexturedShader", "Shaders/DefaultTexturedShader.pxShader");
		shaderLibrary.Load("GridShader", "Shaders/SceneGrid.pxShader");

		shaderLibrary.LoadExternalResource("FinalSceneComposite", "resources/shaders/FinalSceneComposite.pxShader");

		CameraUniformBuffer = UniformBuffer::Create(sizeof(CameraData), 0);
		
		CreateGeoPass();
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


		Renderer2D::BeginRenderPass(s_GeoRenderPass, s_GeoDrawList);
	}

	void SceneRenderer::EndScene()
	{

		FlushDrawLists();
	}

	void SceneRenderer::OnImguiRender()
	{

	}

// 	void SceneRenderer::DrawSceneGrid(float gridAlpha)
// 	{
// 
// // 		s_ShaderLibrary.Get()["GridShader"]->Bind();
// // 
// // 		SceneData->GridBufferData.ViewProj = SceneData->CameraBuffer.ViewProj;
// // 		SceneData->GridBufferData.Zoom = gridAlpha;
// // 		SceneData->GridUniformBufferData->SetData(&SceneData->GridBufferData, sizeof(GridData), 0);
// // 
// // 
// // 		SceneData->SceneGridVertexArray->Bind();
// // 		SceneData->SceneGridVertexArray->GetIbos().Bind();
// // 
// // 		RenderCommand::DrawElements(SceneData->SceneGridVertexArray, PimitiveRenderType::Triangles, SceneData->SceneGridVertexArray->GetIbos().GetCount());
// // 
// // 		s_ShaderLibrary.Get()["GridShader"]->Unbind();
// // 		SceneData->SceneGridVertexArray->Unbind();
// // 		SceneData->SceneGridVertexArray->GetIbos().Unbind();
// 
// 	}

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
		specs.Attachments = { FramebufferTextureFormat::RGBA8,  FramebufferTextureFormat::RED_INT, FramebufferTextureFormat::Depth };
		specs.Width = 960;
		specs.Height = 540;

		s_GeoRenderPass->FrameBufferTarget = Framebuffer::Create(specs);
		FramebufferPool::Add(s_GeoRenderPass->FrameBufferTarget);


		renderPassPool.GenerateDrawList(s_GeoRenderPass, s_GeoDrawList);
	}


	void SceneRenderer::GeoPass()
	{
		Renderer2D::BeginRenderPass(s_GeoRenderPass, s_GeoDrawList);
		
		CameraUniformBuffer->Bind();
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

	
	void SceneRenderer::FlushDrawLists()
	{
		RenderCommand::Clear();
		GetGeometryPass()->FrameBufferTarget->ClearColorAttachment(1, -1);
		GeoPass();
	}

}