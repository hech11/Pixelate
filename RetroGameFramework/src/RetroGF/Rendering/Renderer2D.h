#pragma once

#include "RetroGF/Core/Core.h"
#include "RetroGF/Rendering/Renderable.h"
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

			static void ShutDown() {
			}
			
			static void Begin(RGF::OrthographicCamera* camera);
			static void End();


			static void DrawSprite(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);

			static void ResetStatistics();
			static RenderingStatistics& GetStats();
		private :
			static void Flush();
			static void FlushAndBeginNewBatch();

	};

}