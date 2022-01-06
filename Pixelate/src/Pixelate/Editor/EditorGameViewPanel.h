#pragma once


#include "EditorPanel.h"
#include "Pixelate/Rendering/SceneRenderer.h"

namespace Pixelate
{

	class EditorGameViewPanel : public EditorPanel
	{

		public:
			void OnUpdate(float ts) override;
			void OnImguiRender() override;

			void SetSceneRenderer(const Ref<SceneRenderer>& renderer)
			{
				m_Renderer = renderer;
			}


		private :
			Ref<SceneRenderer> m_Renderer;

			glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
			glm::vec2 m_ViewportPanelSize = { 0.0f, 0.0f };
			glm::vec2 m_ViewportPanelPosition = { 0.0f, 0.0f };

			bool m_IsViewportFocused = false;
			bool m_IsViewportHovered = false;


	};

}
