#pragma once


#include "Pixelate/Rendering/Renderer2D.h"
#include "Pixelate/Scene/Scene.h"
#include "Pixelate/Scene/Components.h"

#include "RenderPass.h"

namespace Pixelate
{

	enum SceneRendererOptions
	{
		RenderCompositeOnly = 0,
		RenderPhysicsPrimitives = BIT(0),
		RenderGrid = BIT(1),
		RenderEntitySelection = BIT(3),
		RenderGizmos = BIT(4),
		RenderAll = RenderPhysicsPrimitives | RenderGrid | RenderEntitySelection | RenderGizmos,
	};



	class SceneRenderer
	{

		public :
			SceneRenderer();

			void Init();

			void SubmitVertices(glm::vec4* vertices, int vertexCount, const glm::vec4& color);
			void SubmitCircle(const glm::vec2& center, float radius, const glm::vec4& color);
			void SubmitAABB(const AABB& aabb, const glm::vec4& color);
			void SubmitSprite(const TransformComponent& transform, const SpriteRendererComponent& sprite, int entityID);
			void SubmitLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color);

			void BeginScene(const glm::mat4& viewProj);
			void EndScene();

			void DrawSceneGrid(float gridAlpha);

			void OnImguiRender();

			Ref<RenderPass>& GetGeometryPass() { return s_GeoRenderPass; }
			void SetOptions(SceneRendererOptions options)
			{
				m_RenderOptions = options;
			}

			SceneRendererOptions GetOptions() const
			{
				return m_RenderOptions;
			}


		private :

			void CreateGeoPass();
			void GeoPass();
			void FlushDrawLists();

		private :
			DrawList s_GeoDrawList;
			Ref<RenderPass> s_GeoRenderPass;
			Ref<DrawData> GridDrawData;

			SceneRendererOptions m_RenderOptions = SceneRendererOptions::RenderCompositeOnly;

			struct CameraData
			{
				glm::mat4 ViewProj;
			};
			CameraData s_CameraBufferData;

			struct GridData
			{
				glm::mat4 ViewProj;
				float Zoom;
			};
			GridData s_GridBufferData;

			Ref<UniformBuffer> CameraUniformBuffer;
			Ref<UniformBuffer> GridUniformBuffer;


	};
}