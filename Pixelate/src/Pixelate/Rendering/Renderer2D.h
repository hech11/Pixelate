#pragma once

#include "Pixelate/Core/Core.h"
#include "Pixelate/Rendering/API/Shader/ShaderManager.h"
#include "Pixelate/Rendering/TextureManager.h"

#include "Pixelate/Rendering/OrthographicCamera.h"
#include "Pixelate/Core/AABB.h"
#include "Pixelate/Core/Rect.h"

#include "Pixelate/Scene/Components.h"


// Renderer2D interface.
// The 'Create' method will decide depending on the API choice. OpenGL, DirectX 11 or 12, Vulkan etc.

// The API equivalent contains a Renderer2D

namespace Pixelate {




	class PX_API Renderer2D  {		

		public :

			struct RenderingStatistics {
				unsigned int IndexCount = 0, VertexSize = 0, DrawCalls = 0,
					MaxVertexBufferSize = 0, MaxIndexBuferSize = 0, MaxSprites = 0;
			};

			static void Init();
			static void ShutDown();
			
			static void BeginScene(Pixelate::OrthographicCamera* camera);
			static void EndScene();


			static void DrawVerticies(glm::vec4* vertices, int vertexCount, const glm::vec4& color);

			static void DrawSprite(const glm::vec3& position, float rotation, const glm::vec3& size, const glm::vec4& color, int entityID);
			static void DrawSprite(const glm::vec3& position, float rotation, const glm::vec3& size, const Ref<Texture>& texture, const glm::vec4& tintColor, int entityID);


			static void DrawSprite(const glm::mat4& transform, const glm::vec4& tintColor, int entityID);
			static void DrawSprite(const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec4& tintColor, int entityID);
			static void DrawSprite(const glm::mat4& transform, const Ref<Texture>& texture, const Rect& textureDestRect, const glm::vec4& tintColor, int entityID);

			static void DrawSprite(const TransformComponent& transform, const SpriteRendererComponent& sprite, int entityID);

			static void DrawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec4& color);
			static void DrawAABB(const AABB& aabb, const glm::vec4& color);

			static void DrawCircle(const glm::vec2& center, float radius, const glm::vec4& color);


			static void SetBoundingBox(bool draw);
			static const bool& ShouldDrawBoundingBox();

			static Ref<Shader>& GetDefaultShader();
			static Ref<Shader>& GetGridShader();


			// temp -- should be in a scene renderer
			static void DrawSceneGrid(float gridAlpha);

			static void ResetStatistics();
			static RenderingStatistics& GetStats();
		private :
			static void BeginNewQuadBatch();
			static void BeginNewLineBatch();

			static std::array<glm::vec2, 4> NormalizedCoordinates(const Rect& rect, const Ref<Texture>& texture);

	};

}