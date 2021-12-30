#pragma once


#include "Pixelate/Rendering/Renderer2D.h"
#include "Pixelate/Scene/Scene.h"
#include "Pixelate/Scene/Components.h"

#include "RenderPass.h"

namespace Pixelate
{

	class SceneRenderer
	{

		public :
			SceneRenderer(Ref<Scene>& scene);

			static void Init();

			static void SetScene(const Ref<Scene>& scene);

			static void SubmitSprite(const TransformComponent& transform, const SpriteRendererComponent& sprite, int entityID);
			static void SubmitLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color);

			static void BeginScene(const glm::mat4& viewProj);
			static void EndScene();


			static void OnImguiRender();


			static void DrawSceneGrid(float gridAlpha);

			static Ref<RenderPass>& GetFinalCompositePass() { return s_CompositePass; }
			static Ref<RenderPass>& GetGeometryPass() { return s_GeoRenderPass; }

		private :
			static void CreateGeoPass();
			static void CreateSceneGridPass();
			static void CreateDebugOutlinePass();

			static void GeoPass();
			static void SceneGridPass();
			static void DebugOutlinePass();

			static void FlushDrawLists();

		private :
			static Ref<Scene> s_CurrentScene;
			static DrawList s_GeoDrawList;

			
			static Ref<RenderPass> s_CompositePass;

			static Ref<RenderPass> s_GeoRenderPass;
			static Ref<RenderPass> s_SceneGridPass;
			static Ref<RenderPass> s_DebuggingOutlinePass;

			// for debugging
			static DrawList s_DebugDrawList;
			static DrawList s_PhysicsDebugDrawList;

			struct CameraData
			{
				glm::mat4 ViewProj;
			};
			static CameraData s_CameraBufferData;

			struct GridData
			{
				glm::mat4 ViewProj;
				float Zoom;
			};
			static GridData s_GridBufferData;

			static Ref<UniformBuffer> CameraUniformBuffer;
			static Ref<UniformBuffer> GridUniformBuffer;


	};
}