#include "PXpch.h"
#include "Renderer2D.h"


#ifdef _DEBUG
#include "Pixelate/Rendering/RendererAPI.h"
#include "Pixelate/Rendering/RenderCommand.h"
#endif // _DEBUG

#include "Pixelate/Debug/Instrumentor.h"
#include "Pixelate/Rendering/TextureManager.h"

#include "Pixelate/Rendering/RendererCapabilities.h"
#include "API/FrameBuffer.h"

#include <math.h>
#include <Pixelate\Core\Timer.h>

#include "Pixelate/Scene/Components.h"
#include "Pixelate/rendering/RenderPass.h"
#include "SceneRenderer.h"


namespace Pixelate {

	
	struct Renderer2DData {

		Ref<RenderPass> CurrentRenderPass;


		Ref<DrawData> LineDrawData;
		Ref<DrawData> VertexDrawData;
		Ref<DrawData> LineStripDrawData;

		// For drawing the scene grid.
		Ref<VertexArray> SceneGridVertexArray;
		Ref<VertexBuffer> SceneGridVertexBuffer;
		Ref<UniformBuffer> GridUniformBufferData;


		std::array<glm::vec4, 4> QuadPivotPointPositions;

	};

	static Renderer2DData* SceneData = nullptr;
	ShaderLibrary Renderer2D::s_ShaderLibrary;
	RenderPassPool Renderer2D::s_RPPool;


	int RendererCapabilities::MaxTextureSlots = 0; // This may need to be moved into its own translation unit potentially in the future


	void Renderer2D::Init() {
		PX_PROFILE_FUNCTION();
		SceneData = new Renderer2DData;
		FramebufferPool::Init();

		SceneData->LineDrawData = CreateRef<DrawData>();
		SceneData->LineStripDrawData =  CreateRef<DrawData>();
		SceneData->VertexDrawData = CreateRef<DrawData>();


		SceneData->LineDrawData->MaxVertexSize = SceneData->LineDrawData->MaxDrawObjectSize * 2;
		SceneData->LineStripDrawData->MaxVertexSize = SceneData->LineDrawData->MaxDrawObjectSize * 2;

		SceneData->LineDrawData->Invalidate(PimitiveRenderType::Lines);
		SceneData->LineStripDrawData->Invalidate(PimitiveRenderType::LineStrip);


		SceneData->VertexDrawData->Invalidate();

		TextureManager::Init();

		// Vertex order = bottom left -> bottom right -> top right -> top left
		SceneData->QuadPivotPointPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f }; // -- bottom left
		SceneData->QuadPivotPointPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f}; // -- bottom right
		SceneData->QuadPivotPointPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f}; // -- top right
		SceneData->QuadPivotPointPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f}; // -- top left



// 
// 		float gridVerticies[] = {
// 			 -10000.0f, -10000.0f, 0.0f, 0.0f, 0.0f,
// 			  10000.0f, -10000.0f, 0.0f, 1.0f, 0.0f,
// 			  10000.0f,  10000.0f, 0.0f, 1.0f, 1.0f,
// 			 -10000.0f,  10000.0f, 0.0f, 0.0f, 1.0f
// 
// 		};
// 		SceneData->SceneGridVertexArray = VertexArray::Create();
// 		SceneData->SceneGridVertexBuffer = VertexBuffer::Create(gridVerticies,  sizeof(gridVerticies));
// 		BufferLayout layout = {
// 			{BufferLayoutTypes::Float3, "aPos"},
// 			{BufferLayoutTypes::Float2, "aUV" }
// 		};
// 
// 
// 		SceneData->SceneGridVertexBuffer->SetLayout(layout);
// 		SceneData->SceneGridVertexArray->PushVertexBuffer(SceneData->SceneGridVertexBuffer);
// 
// 
// 		unsigned int gridIndicies[] = {
// 			0, 1, 2,
// 			2, 3, 0
// 		};
// 		Ref<IndexBuffer> aaa = IndexBuffer::Create(gridIndicies, 6);
// 		SceneData->SceneGridVertexArray->PushIndexBuffer(aaa);
// 
// 		SceneData->GridUniformBufferData = UniformBuffer::Create(sizeof(GridData), 0);
		
	}

	void Renderer2D::ShutDown() {
		TextureManager::Shutdown();
		FramebufferPool::Destroy();
		s_RPPool.Destroy();

		delete SceneData;
	}

	void Renderer2D::BeginScene() {
		PX_PROFILE_FUNCTION();

		SceneData->LineDrawData->IndexCount = 0;
		SceneData->LineDrawData->PtrData = SceneData->LineDrawData->PtrBase;


		SceneData->LineStripDrawData->IndexCount = 0;
		SceneData->LineStripDrawData->PtrData = SceneData->LineDrawData->PtrBase;

		SceneData->VertexDrawData->IndexCount = 0;
		SceneData->VertexDrawData->PtrData = SceneData->VertexDrawData->PtrBase;

	}

	void Renderer2D::FlushPrimitives() {
		PX_PROFILE_FUNCTION();
		

		uint32_t lineSize = (uint8_t*)SceneData->LineDrawData->PtrData - (uint8_t*)SceneData->LineDrawData->PtrBase;
		if (lineSize) {
			SceneData->LineDrawData->Vao->Bind();
			SceneData->LineDrawData->Vao->GetIbos().Bind();
			SceneData->LineDrawData->Vbo->SetData(SceneData->LineDrawData->PtrBase, lineSize);

			TextureManager::GetDefaultTexture()->Bind();
			RenderCommand::SetLineThickness(1.0f);

			RenderCommand::DrawElements(SceneData->LineDrawData->Vao, PimitiveRenderType::Lines, SceneData->LineDrawData->IndexCount);

		}


		TextureManager::GetDefaultTexture()->Bind();
		RenderCommand::SetLineThickness(1.0f);

		for (int i = 0; i < SceneData->LineStripDrawData->Iterations; i++) {

			uint32_t lineStripSize = (uint8_t*)SceneData->LineStripDrawData->PtrData - (uint8_t*)SceneData->LineStripDrawData->PtrBase;
			uint32_t size = lineStripSize / SceneData->LineStripDrawData->Iterations;

			VertexData* ptrBase = SceneData->LineStripDrawData->PtrBase;
			uint32_t indexCount = SceneData->LineStripDrawData->IndexCount / SceneData->LineStripDrawData->Iterations;

			ptrBase += i*indexCount;

			
			if (lineStripSize) {
				SceneData->LineStripDrawData->Vao->Bind();
				SceneData->LineStripDrawData->Vao->GetIbos().Bind();
				SceneData->LineStripDrawData->Vbo->SetData(ptrBase, size);

				RenderCommand::DrawElements(SceneData->LineStripDrawData->Vao, PimitiveRenderType::LineStrip, indexCount);

			}

		}


	}


	void Renderer2D::SetCurrentRenderPassTarget(const Ref<RenderPass>& renderPass)
	{
		SceneData->CurrentRenderPass = renderPass;
	}

	void Renderer2D::DrawVerticies(glm::vec4* vertices, int vertexCount, const glm::vec4& color) {

		if (SceneData->VertexDrawData->IndexCount >= SceneData->VertexDrawData->MaxIndexSize) {
			SceneData->VertexDrawData->PrepareNewBatch();
		}

		unsigned char r = color.x * 255.0f;
		unsigned char g = color.y * 255.0f;
		unsigned char b = color.z * 255.0f;
		unsigned char a = color.w * 255.0f;


		unsigned int col = a << 24 | b << 16 | g << 8 | r;

		// Vertex order = bottom left -> bottom right -> top right -> top left
		for (unsigned int i = 0; i < vertexCount; i++) {

			SceneData->VertexDrawData->PtrData->Verticies = vertices[i];
			SceneData->VertexDrawData->PtrData->Color = col;
			SceneData->VertexDrawData->PtrData->TextureCoords = {0.0f, 0.0f};
			SceneData->VertexDrawData->PtrData->TextureIndex = 0.0f;
			SceneData->VertexDrawData->PtrData->EntityID = -1;
			SceneData->VertexDrawData->PtrData++;
		}

		if (vertexCount < 4) {
			glm::vec4 v = vertices[2];
			SceneData->VertexDrawData->PtrData->Verticies = v;
			SceneData->VertexDrawData->PtrData->Color = col;
			SceneData->VertexDrawData->PtrData->TextureCoords = { 0.0f, 0.0f };
			SceneData->VertexDrawData->PtrData->TextureIndex = 0.0f;
			SceneData->VertexDrawData->PtrData->EntityID = -1;
			SceneData->VertexDrawData->PtrData++;
		}


		SceneData->VertexDrawData->IndexCount += 6;

	}



	void Renderer2D::BeginRenderPass(const Ref<RenderPass>& renderPass, const DrawList& list)
	{
		s_RPPool.GenerateDrawList(renderPass, list);
		renderPass->FrameBufferTarget->Bind();


		SetCurrentRenderPassTarget(renderPass);

	}
	void Renderer2D::EndRenderPass(const Ref<RenderPass>& renderPass)
	{
		renderPass->FrameBufferTarget->Unbind();
	}

	void Renderer2D::DrawSpriteWithShader(const glm::mat4& transform, const Ref<Texture>& texture, const Rect& textureDestRect, const glm::vec4& tintColor, const Ref<Shader>& shader, int entityID)
	{
		auto& renderPassPool = s_RPPool.GetPool()[SceneData->CurrentRenderPass];
		auto& command = renderPassPool[shader];

		if (command->IndexCount >= command->MaxIndexSize)
			command->PrepareNewBatch();

		constexpr unsigned int VertexCount = 4;


		unsigned char r = tintColor.r * 255.0f;
		unsigned char g = tintColor.g * 255.0f;
		unsigned char b = tintColor.b * 255.0f;
		unsigned char a = tintColor.a * 255.0f;

		unsigned int color = a << 24 | b << 16 | g << 8 | r;

		float textureIndex = 0.0f;
		std::array<glm::vec2, 4> texCoords = { {{0.0f, 0.0f}, {0.0f, 0.0f},{0.0f, 0.0f},{0.0f, 0.0f}}};
		if (texture)
		{
			textureIndex = TextureManager::IsTextureValid(texture);

			if (textureIndex == 0.0f) {
				auto& manager = TextureManager::GetManagerData();
				if (manager.TextureSlotIndex >= RendererCapabilities::MaxTextureSlots)
					command->PrepareNewBatch();
				textureIndex = manager.TextureSlotIndex;
				TextureManager::DirectAdd(texture);
			}

			texCoords = NormalizedCoordinates(textureDestRect, texture);
		}
		

		for (unsigned int i = 0; i < VertexCount; i++) {
			
			command->PtrData->Verticies = transform * SceneData->QuadPivotPointPositions[i];
			command->PtrData->Color = color;
			command->PtrData->TextureCoords = texCoords[i];
			command->PtrData->TextureIndex = textureIndex;
			command->PtrData->EntityID = entityID;
			command->PtrData++;
		}


		command->IndexCount += 6;
	}

	void Renderer2D::DrawSpriteWithShader(const TransformComponent& transform, const SpriteRendererComponent& sprite, int entityID)
	{
		DrawSpriteWithShader(transform.Transform, sprite.Texture, sprite.Rect, sprite.TintColor, sprite.Shader, entityID);

	}


	void Renderer2D::DrawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec4& color)
	{
		PX_PROFILE_FUNCTION();


		if (SceneData->LineDrawData->IndexCount >= SceneData->LineDrawData->MaxIndexSize)
			SceneData->LineDrawData->PrepareNewBatch();

		unsigned char r = color.r * 255.0f;
		unsigned char g = color.g * 255.0f;
		unsigned char b = color.b * 255.0f;
		unsigned char a = color.a * 255.0f;

		unsigned int c = a << 24 | b << 16 | g << 8 | r;

		
		SceneData->LineDrawData->PtrData->Verticies = p1;
		SceneData->LineDrawData->PtrData->Color = c;
		SceneData->LineDrawData->PtrData->TextureIndex = 0.0f;
		SceneData->LineDrawData->PtrData->EntityID = -1;
		SceneData->LineDrawData->PtrData++;

		SceneData->LineDrawData->PtrData->Verticies = p2;
		SceneData->LineDrawData->PtrData->Color = c;
		SceneData->LineDrawData->PtrData->TextureIndex = 0.0f;
		SceneData->LineDrawData->PtrData->EntityID = -1;
		SceneData->LineDrawData->PtrData++;


		SceneData->LineDrawData->IndexCount += 2;
		SceneData->LineDrawData->Iterations += 1;

	}

	void Renderer2D::DrawAABB(const AABB& aabb, const glm::vec4& color)
	{
		glm::vec3 corners[4] =
		{
			glm::vec3 { aabb.Min.x, aabb.Min.y, aabb.Max.z},
			glm::vec3 { aabb.Min.x, aabb.Max.y, aabb.Max.z},
			glm::vec3 { aabb.Max.x, aabb.Max.y, aabb.Max.z},
			glm::vec3 { aabb.Max.x, aabb.Min.y, aabb.Max.z},

		};

		for (uint32_t i = 0; i < 4; i++)
			Renderer2D::DrawLine(corners[i], corners[(i + 1) % 4], color);

	}

	void Renderer2D::DrawCircle(const glm::vec2& center, float radius, const glm::vec4& color) {

		if (SceneData->LineStripDrawData->IndexCount >= SceneData->LineStripDrawData->MaxIndexSize)
			SceneData->LineStripDrawData->PrepareNewBatch();


		float pi2 = 2.0f * 3.14f;
		constexpr int sides = 64;
		constexpr int verts = sides + 1;
		
		uint8_t r = color.r * 255.0f;
		uint8_t g = color.g * 255.0f;
		uint8_t b = color.b * 255.0f;
		uint8_t a = color.a * 255.0f;

		uint32_t c = a << 24 | b << 16 | g << 8 | r;


		for (uint32_t angle = 0; angle < verts; angle++) {
			glm::vec3 result;
			result.x = center.x + (radius * cos(angle * pi2 / sides));
			result.y = center.y + (radius * sin(angle * pi2 / sides));
			result.z = 0.0f;


			SceneData->LineStripDrawData->PtrData->Verticies = result;
			SceneData->LineStripDrawData->PtrData->Color = c;
			SceneData->LineStripDrawData->PtrData->TextureCoords = { 0.0f, 0.0f };
			SceneData->LineStripDrawData->PtrData->TextureIndex = 0.0f;
			SceneData->LineStripDrawData->PtrData->EntityID = -1;
			SceneData->LineStripDrawData->PtrData++;


			SceneData->LineStripDrawData->IndexCount++;


		}

		SceneData->LineStripDrawData->Iterations++;

	}


	std::array<glm::vec2, 4> Renderer2D::NormalizedCoordinates(const Rect& rect, const Ref<Texture>& texture) {
		std::array<glm::vec2, 4> result;

		float xOffset = (float)rect.Position.x / texture->GetWidth();
		float yOffset = (float)rect.Position.y / texture->GetHeight();

		float xSize = (float)rect.Scale.x / texture->GetWidth();
		float ySize = (float)rect.Scale.y / texture->GetHeight();



		result[0] = { xOffset, 1.0f - (yOffset + ySize) }; // bottom left
		result[1] = { xOffset + xSize, 1.0f - (yOffset + ySize) }; // bottom right
		result[2] = { xOffset + xSize, 1.0f - yOffset }; // top right
		result[3] = { xOffset, 1.0f - yOffset }; // top left

		return result;

	}
	
}