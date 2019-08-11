#pragma once

#include "RetroGF/Core.h"
#include "RetroGF/Rendering/Renderable.h"
#include "RetroGF/Rendering/Material.h"
#include "RetroGF/Rendering/API/Shader/ShaderManager.h"

#include "RetroGF/Rendering/OrthographicCamera.h"



// Renderer2D interface.
// The 'Create' method will decide depending on the API choice. OpenGL, Directx 11 or 12, vulkan etc.

// The API equivlent contains a Renderer2D


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
				delete m_Vao;
				delete m_Vbo;
				delete m_Ibo;

			}
			
			static void Start(RGF::OrthographicCamera* camera);
			static void End();


			static void Submit(const Renderable* renderable, Shader* currentShader);
			static void Render();

			static MaterialManager& GetMaterialManager() { return *s_MatManager; }
			static ShaderManager& GetShaderManager() { return *s_ShaderManager; }

		private :
			struct SceneData {
				glm::mat4 ViewProjectionMatrix;
				std::vector<Material*> mats;
			};

			static SceneData* m_SceneData;

			static VertexArray* m_Vao;
			static VertexBuffer* m_Vbo;
			static IndexBuffer* m_Ibo;


			static VertexData* Buffer;
			static unsigned int m_IndexCount;


			static std::unique_ptr<MaterialManager> s_MatManager;
			static std::unique_ptr<ShaderManager> s_ShaderManager;
	};

}