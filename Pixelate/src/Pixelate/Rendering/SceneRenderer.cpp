#include "PXpch.h"
#include "SceneRenderer.h"

#include "Pixelate/Rendering/TextureManager.h"
#include "MaterialManager.h"


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



		// Init the scene grid
		GridDrawData = CreateRef<DrawData>();
		float gridVerticies[] = {
			 -10000.0f, -10000.0f, 0.0f, 0.0f, 0.0f,
			  10000.0f, -10000.0f, 0.0f, 1.0f, 0.0f,
			  10000.0f,  10000.0f, 0.0f, 1.0f, 1.0f,
			 -10000.0f,  10000.0f, 0.0f, 0.0f, 1.0f

		};
		GridDrawData->Vao = VertexArray::Create();
		GridDrawData->Vbo = VertexBuffer::Create(gridVerticies,  sizeof(gridVerticies));
		BufferLayout layout = {
			{BufferLayoutTypes::Float3, "aPos"},
			{BufferLayoutTypes::Float2, "aUV" }
		};


		GridDrawData->Vbo->SetLayout(layout);
		GridDrawData->Vao->PushVertexBuffer(GridDrawData->Vbo);


		uint32_t gridIndicies[] = {
			0, 1, 2,
			2, 3, 0
		};
		Ref<IndexBuffer> ibo = IndexBuffer::Create(gridIndicies, 6);
		GridDrawData->Vao->PushIndexBuffer(ibo);
		GridUniformBuffer = UniformBuffer::Create(sizeof(GridData), 0);
		GridDrawData->Vao->Unbind();

		
	}


	void SceneRenderer::SubmitVertices(glm::vec4* vertices, int vertexCount, const glm::vec4& color)
	{
		if (s_GeoDrawList.find(DefaultMaterial) == s_GeoDrawList.end())
		{
			s_GeoDrawList[DefaultMaterial] = CreateRef<DrawData>();
			s_GeoDrawList[DefaultMaterial]->Invalidate();
		}
		Renderer2D::BeginRenderPass(s_GeoRenderPass, s_GeoDrawList);
		Renderer2D::DrawVerticies(vertices, vertexCount, color);

	}

	void SceneRenderer::SubmitCircle(const glm::vec2& center, float radius, const glm::vec4& color)
	{
		if (s_GeoDrawList.find(DefaultMaterial) == s_GeoDrawList.end())
		{
			s_GeoDrawList[DefaultMaterial] = CreateRef<DrawData>();
			s_GeoDrawList[DefaultMaterial]->Invalidate();
		}
		Renderer2D::BeginRenderPass(s_GeoRenderPass, s_GeoDrawList);
		Renderer2D::DrawCircle(center, radius, color);

	}

	void SceneRenderer::SubmitAABB(const AABB& aabb, const glm::vec4& color)
	{
		if (s_GeoDrawList.find(DefaultMaterial) == s_GeoDrawList.end())
		{
			s_GeoDrawList[DefaultMaterial] = CreateRef<DrawData>();
			s_GeoDrawList[DefaultMaterial]->Invalidate();
		}
		Renderer2D::BeginRenderPass(s_GeoRenderPass, s_GeoDrawList);

		Renderer2D::DrawAABB(aabb, color);
	}

	void SceneRenderer::SubmitSprite(const TransformComponent& transform, const SpriteRendererComponent& sprite, int entityID)
	{

		auto& material = sprite.Material;
		if (s_GeoDrawList.find(material) == s_GeoDrawList.end())
		{
			s_GeoDrawList[material ] = CreateRef<DrawData>();
			s_GeoDrawList[material ]->Invalidate();
		}

		Renderer2D::BeginRenderPass(s_GeoRenderPass, s_GeoDrawList);
		Renderer2D::DrawSpriteWithShader(transform, sprite, entityID);

	}

	void SceneRenderer::SubmitLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4 & color)
	{

		if (s_GeoDrawList.find(DefaultMaterial) == s_GeoDrawList.end())
		{
			s_GeoDrawList[DefaultMaterial] = CreateRef<DrawData>();
			s_GeoDrawList[DefaultMaterial]->Invalidate();
		}

		Renderer2D::BeginRenderPass(s_GeoRenderPass, s_GeoDrawList);
		Renderer2D::DrawLine(p0, p1, color);
	}

	void SceneRenderer::BeginScene(const glm::mat4& viewProj)
	{
		s_CameraBufferData.ViewProj = viewProj;

		Renderer2D::BeginPrimitives();

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

 	void SceneRenderer::DrawSceneGrid(float gridAlpha)
 	{

		s_GridBufferData.ViewProj = s_CameraBufferData.ViewProj;
		s_GridBufferData.Zoom = gridAlpha;

		GridUniformBuffer->Bind();
		GridUniformBuffer->SetData(&s_GridBufferData, sizeof(GridData), 0);

		Ref<Shader>& gridShader = Renderer2D::GetShaderLibrary().Get()["GridShader"];
		gridShader->Bind();


		GridDrawData->Vao->Bind();
		GridDrawData->Vao->GetIbos().Bind();

		RenderCommand::DrawElements(GridDrawData->Vao, PimitiveRenderType::Triangles, GridDrawData->Vao ->GetIbos().GetCount());

		gridShader->Unbind();
		GridDrawData->Vao->Unbind();
		GridDrawData->Vao->GetIbos().Unbind();


 	}

	void SceneRenderer::CreateGeoPass()
	{
		auto& renderPassPool = Renderer2D::GetRenderPassPool();
		auto& shaderLibrary = Renderer2D::GetShaderLibrary();

		DefaultMaterial = MaterialManager::LoadExternalResource("resources/materials/DefaultMaterial.pxMaterial");


		s_GeoDrawList[DefaultMaterial] = CreateRef<DrawData>();
		auto& drawData = s_GeoDrawList[DefaultMaterial];
		drawData->Invalidate();

		s_GeoRenderPass = CreateRef<RenderPass>();
		s_GeoRenderPass->DebugName = "GeoPass";
		s_GeoRenderPass->Material = DefaultMaterial;

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
		
		

		for (auto& [material, command] : s_GeoDrawList)
 		{
			
			material->Bind();
			material->Set("u_ViewProjection", s_CameraBufferData);
			material->UpdateMaterial();


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

		Renderer2D::FlushPrimitives();
		Renderer2D::EndRenderPass(s_GeoRenderPass);

	}

	
	void SceneRenderer::FlushDrawLists()
	{

		GetGeometryPass()->FrameBufferTarget->ClearColorAttachment(1, -1);
		GeoPass();
	}

}