#pragma once

#include "Pixelate/Core/Core.h"
#include "Pixelate/Rendering/API/Shader/ShaderLibrary.h"
#include "Pixelate/Rendering/TextureManager.h"

#include "Pixelate/Rendering/OrthographicCamera.h"
#include "Pixelate/Core/AABB.h"
#include "Pixelate/Core/Rect.h"

#include "Pixelate/Rendering/RendererAPI.h"
#include "Pixelate/Rendering/RenderCommand.h"

#include "Pixelate/Rendering/Material.h"


// Renderer2D interface.
// The 'Create' method will decide depending on the API choice. OpenGL, DirectX 11 or 12, Vulkan etc.

// The API equivalent contains a Renderer2D

namespace Pixelate {



	struct TransformComponent;
	struct SpriteRendererComponent;

	class RenderPass;
	class RenderPassPool;




	struct PX_API VertexData {
		glm::vec3 Verticies;
		unsigned int Color;
		glm::vec2 TextureCoords;
		float TextureIndex;
		int EntityID;
	};


	struct DrawData
	{
		uint32_t MaxDrawObjectSize = 10000;
		uint32_t MaxVertexSize = MaxDrawObjectSize * 4;
		uint32_t MaxIndexSize = MaxDrawObjectSize * 6;

		uint32_t Iterations = 0;

		Ref<VertexArray> Vao;
		Ref<VertexBuffer> Vbo;
		Ref<UniformBuffer> Ubo;

		BufferLayout Layout;

		uint32_t IndexCount = 0;

		VertexData* PtrBase = nullptr, * PtrData = nullptr;

		PimitiveRenderType PrimitiveType;

		void Invalidate(PimitiveRenderType type = PimitiveRenderType::Triangles)
		{

			PrimitiveType = type;
			Vao = VertexArray::Create();
			Vbo = VertexBuffer::Create(MaxVertexSize * sizeof(VertexData));


			Layout =
			{
				{ BufferLayoutTypes::Float3, "aPos"},
				{ BufferLayoutTypes::UChar4, "aColor", true},
				{ BufferLayoutTypes::Float2, "aTexCoords"},
				{ BufferLayoutTypes::Float, "aTexIndex"},
				{ BufferLayoutTypes::Int, "aEntityID" }

			};


			Vbo->SetLayout(Layout);
			Vao->PushVertexBuffer(Vbo);

			uint32_t* indices = new uint32_t[MaxIndexSize];
			if (type == PimitiveRenderType::Triangles)
			{
				int offset = 0;
				for (uint32_t i = 0; i < MaxIndexSize; i += 6) {
					indices[i] = offset + 0;
					indices[i + 1] = offset + 1;
					indices[i + 2] = offset + 2;
					indices[i + 3] = offset + 2;

					indices[i + 4] = offset + 3;
					indices[i + 5] = offset + 0;

					offset += 4;
				}

			}
			else
			{
				for (uint32_t i = 0; i < MaxIndexSize; i++)
					indices[i] = i;
			}

			Ref<IndexBuffer> ibo = IndexBuffer::Create(indices, MaxIndexSize);
			Vao->PushIndexBuffer(ibo);
			Vao->Unbind();
			delete[] indices;



			if (PtrBase)
				delete PtrBase;

			PtrBase = new VertexData[MaxVertexSize];
			IndexCount = 0;
			PtrData = PtrBase;
		}

		void PrepareNewBatch()
		{
			uint32_t ptrSize = (uint8_t*)PtrData - (uint8_t*)PtrBase;

			Vao->Bind();
			Vbo->Bind();
			Vbo->SetData(PtrBase, ptrSize);
			Vao->GetIbos().Bind();
			TextureManager::BindAllTextures();
			RenderCommand::SetLineThickness(1.0f);

			if (PrimitiveType == PimitiveRenderType::LineStrip)
			{
				for (int i = 0; i < Iterations; i++) {

					uint32_t size = ptrSize / Iterations;

					VertexData* ptrBase = PtrBase;
					uint32_t indexCount = IndexCount / Iterations;

					ptrBase += i * indexCount;

					RenderCommand::DrawElements(Vao, PimitiveRenderType::LineStrip, indexCount);

				}
			}
			else
			{
				RenderCommand::DrawElements(Vao, PrimitiveType, IndexCount);
			}


			IndexCount = 0;
			Iterations = 0;
			PtrData = PtrBase;
			TextureManager::GetManagerData().TextureSlotIndex = 1;

		}

	};



	using DrawList = std::unordered_map<Ref<Material>, Ref<DrawData>>;


	class PX_API Renderer2D  {

		public :

			static void Init();
			static void ShutDown();
			
			static void BeginPrimitives();
			static void FlushPrimitives();

			static void SetCurrentRenderPassTarget(const Ref<RenderPass>& renderPass);

			static void DrawVerticies(glm::vec4* vertices, int vertexCount, const glm::vec4& color);

			
			static void DrawSpriteWithShader(const glm::mat4& transform, const Ref<Texture>& texture, const Rect& textureDestRect, const glm::vec4& tintColor, const Ref<Material>& material, int entityID);
			static void DrawSpriteWithShader(const TransformComponent& transform, const SpriteRendererComponent& sprite, int entityID);


			static void DrawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec4& color);
			static void DrawAABB(const AABB& aabb, const glm::vec4& color);
			static void DrawCircle(const glm::vec2& center, float radius, const glm::vec4& color);


			static ShaderLibrary& GetShaderLibrary() { return s_ShaderLibrary; }
			static RenderPassPool& GetRenderPassPool() { return s_RPPool; }


			static void BeginRenderPass(const Ref<RenderPass>& renderPass, const DrawList& list);
			static void EndRenderPass(const Ref<RenderPass>& renderPass);

		private :
			static std::array<glm::vec2, 4> NormalizedCoordinates(const Rect& rect, const Ref<Texture>& texture);
		private :
			static ShaderLibrary s_ShaderLibrary;
			static RenderPassPool s_RPPool;
	};



	

}
