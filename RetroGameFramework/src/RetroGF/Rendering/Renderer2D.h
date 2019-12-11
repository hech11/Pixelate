#pragma once

#include "RetroGF/Core/Core.h"
#include "RetroGF/Rendering/Renderable.h"
#include "RetroGF/Rendering/Material.h"
#include "RetroGF/Rendering/API/Shader/ShaderManager.h"
#include "RetroGF/Rendering/API/TextureManager.h"

#include "RetroGF/Rendering/OrthographicCamera.h"



// Renderer2D interface.
// The 'Create' method will decide depending on the API choice. OpenGL, DirectX 11 or 12, Vulkan etc.

// The API equivalent contains a Renderer2D


#define RENDERER_MAX_SPRITES 10000
#define RENDERER_VERTEX_SIZE sizeof(VertexData)
#define RENDERER_OBJECT_SIZE (RENDERER_VERTEX_SIZE  * 4)
#define RENDERER_BUFFER_SIZE (RENDERER_OBJECT_SIZE * RENDERER_MAX_SPRITES)
#define RENDERER_INDICIES_SIZE (RENDERER_MAX_SPRITES * 6)

namespace RGF {




	class RGF_API Renderer2D  {		

		public :

			static void Init();

			static void ShutDown() {
			}
			
			static void Start(RGF::OrthographicCamera* camera);
			static void End();


			static void Submit(const Ref<Renderable>& renderable);
			static void Render();

			static MaterialManager& GetMaterialManager() { return *s_MatManager; }
			static ShaderManager& GetShaderManager() { return *s_ShaderManager; }
			static TextureManager& GetTextureManager() { return *s_TextureManager; }

		private :
			struct SceneData {
				glm::mat4 ViewProjectionMatrix;
				std::vector<Material*> mats;
			};

			static SceneData* m_SceneData;

			static Ref<VertexArray> m_Vao;
			static Ref<VertexBuffer> m_Vbo;
			static Ref<IndexBuffer> m_Ibo;


			static VertexData* Buffer;
			static unsigned int m_IndexCount;


			static Scoped<MaterialManager> s_MatManager;
			static Scoped<ShaderManager> s_ShaderManager;
			static Scoped<TextureManager> s_TextureManager;
	};

}