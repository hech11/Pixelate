#pragma once

#include "Pixelate/Core/Core.h"
#include "Pixelate/Rendering/API/Shader/ShaderLibrary.h"
#include "Pixelate/Rendering/TextureManager.h"

#include "Pixelate/Rendering/OrthographicCamera.h"
#include "Pixelate/Core/AABB.h"
#include "Pixelate/Core/Rect.h"

#include "Pixelate/Rendering/RendererAPI.h"
#include "Pixelate/Rendering/RenderCommand.h"



// Renderer2D interface.
// The 'Create' method will decide depending on the API choice. OpenGL, DirectX 11 or 12, Vulkan etc.

// The API equivalent contains a Renderer2D

namespace Pixelate {


	struct TransformComponent;
	struct SpriteRendererComponent;

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

		Ref<VertexArray> Vao;
		Ref<VertexBuffer> Vbo;
		Ref<UniformBuffer> Ubo;


		uint32_t IndexCount = 0;

		VertexData* PtrBase = nullptr, * PtrData = nullptr;

		void Invalidate()
		{
			Vao = VertexArray::Create();
			Vbo = VertexBuffer::Create(MaxVertexSize * sizeof(VertexData));


			BufferLayout layout =
			{
				{ BufferLayoutTypes::Float3, "aPos"},
				{ BufferLayoutTypes::UChar4, "aColor", true},
				{ BufferLayoutTypes::Float2, "aTexCoords"},
				{ BufferLayoutTypes::Float, "aTexIndex"},
				{ BufferLayoutTypes::Int, "aEntityID" }

			};


			Vbo->SetLayout(layout);
			Vao->PushVertexBuffer(Vbo);


			uint32_t* indices = new uint32_t[MaxIndexSize];
			int offset = 0;
			for (unsigned int i = 0; i < MaxIndexSize; i += 6) {
				indices[i] = offset + 0;
				indices[i + 1] = offset + 1;
				indices[i + 2] = offset + 2;
				indices[i + 3] = offset + 2;

				indices[i + 4] = offset + 3;
				indices[i + 5] = offset + 0;

				offset += 4;
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
	};


	class PX_API Renderer2D  {		

		public :


			static void Init();
			static void ShutDown();
			
			static void BeginScene(Pixelate::OrthographicCamera* camera);
			static void EndScene();


			static void DrawVerticies(glm::vec4* vertices, int vertexCount, const glm::vec4& color);

			
			static void DrawSpriteWithShader(const glm::mat4& transform, const Ref<Texture>& texture, const Rect& textureDestRect, const glm::vec4& tintColor, const Ref<Shader>& shader, int entityID);
			static void DrawSpriteWithShader(const TransformComponent& transform, const SpriteRendererComponent& sprite, int entityID);

			static void DrawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec4& color);
			static void DrawAABB(const AABB& aabb, const glm::vec4& color);

			static void DrawCircle(const glm::vec2& center, float radius, const glm::vec4& color);


			static void SetBoundingBox(bool draw);
			static const bool& ShouldDrawBoundingBox();

			static ShaderLibrary& GetShaderLibrary() { return s_ShaderLibrary; }

			static std::unordered_map<Ref<Shader>, Ref<DrawData>>& GetDrawList();

			// temp -- should be in a scene renderer
			static void DrawSceneGrid(float gridAlpha);

			static void ResetStatistics();
		private :
			static void BeginNewQuadBatch();
			static void BeginNewLineBatch();

			static std::array<glm::vec2, 4> NormalizedCoordinates(const Rect& rect, const Ref<Texture>& texture);
		private :
			static ShaderLibrary s_ShaderLibrary;
	};

}
