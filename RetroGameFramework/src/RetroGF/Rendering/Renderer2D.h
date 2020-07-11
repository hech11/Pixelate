#pragma once

#include "RetroGF/Core/Core.h"
#include "RetroGF/Rendering/API/Shader/ShaderManager.h"
#include "RetroGF/Rendering/API/TextureManager.h"

#include "RetroGF/Rendering/OrthographicCamera.h"



// Renderer2D interface.
// The 'Create' method will decide depending on the API choice. OpenGL, DirectX 11 or 12, Vulkan etc.

// The API equivalent contains a Renderer2D

namespace RGF {

	class RGF_API Renderer2D  {		

		public :

			struct RenderingStatistics {
				unsigned int IndexCount = 0, VertexSize = 0, DrawCalls = 0,
					MaxVertexBufferSize = 0, MaxIndexBuferSize = 0, MaxSprites = 0;
			};
			static void Init();
			static void ShutDown();
			
			static void BeginScene(RGF::OrthographicCamera* camera);
			static void EndScene();


			static void DrawVerticies(const std::array<glm::vec4, 4>& vertices, int vertexCount, const glm::vec4& color);
			static void DrawSprite(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);
			static void DrawSprite(const glm::vec3& position, float rotation, const glm::vec3& size, const glm::vec4& color);
			static void DrawSprite(const glm::vec3& position, const glm::vec3& size, const Ref<Texture>& texture, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
			static void DrawSprite(const glm::vec3& position, float rotation, const glm::vec3& size, const Ref<Texture>& texture, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });

			static void DrawSprite(const glm::vec3& position, const glm::vec3& size, const Ref<TextureBounds>& textureBounds, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
			static void DrawSprite(const glm::vec3& position, float rotation, const glm::vec3& size, const Ref<TextureBounds>& textureBounds, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });


			static void ResetStatistics();
			static RenderingStatistics& GetStats();
		private :
			static void Flush();
			static void FlushAndBeginNewBatch();

	};

}