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
			SceneRenderer();

			void Init();


			void SubmitSprite(const TransformComponent& transform, const SpriteRendererComponent& sprite, int entityID);
			void SubmitLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color);

			void BeginScene(const glm::mat4& viewProj);
			void EndScene();


			void OnImguiRender();

			Ref<RenderPass>& GetGeometryPass() { return s_GeoRenderPass; }

		private :
			void CreateGeoPass();
			void GeoPass();
			void FlushDrawLists();

		private :
			DrawList s_GeoDrawList;
			Ref<RenderPass> s_GeoRenderPass;


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