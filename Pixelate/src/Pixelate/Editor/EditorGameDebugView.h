#pragma once


#include "EditorPanel.h"
#include "Pixelate/Rendering/SceneRenderer.h"

#include "Pixelate/Events/Event.h"
#include "Pixelate/Events/KeyEvents.h"
#include "Pixelate/Events/MouseEvents.h"

#include "Pixelate/Editor/EditorCamera.h"

namespace Pixelate
{
	class EditorGameDebugView : public EditorPanel
	{
		public:
			EditorGameDebugView();

			void OnUpdate(float ts) override;
			void OnImguiRender() override;

			void SetSceneRenderer(const Ref<SceneRenderer>& renderer)
			{
				m_Renderer = renderer;
			}


			void OnEvent(Event& e) override;

			void SetEditorCamera(const Ref<EditorCamera>& camera)
			{
				m_EditorCamera = camera;
			}

			bool IsPanelFocused() const { return m_IsViewportFocused; }
			const Ref<EditorCamera>& GetEditorCamera() const { return m_EditorCamera; }

		private :
			bool OnKeyPressedEvent(KeyPressedEvent& e);
			bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		private:
			Ref<SceneRenderer> m_Renderer;
			Ref<EditorCamera> m_EditorCamera;

			glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
			glm::vec2 m_ViewportPanelSize = { 0.0f, 0.0f };
			glm::vec2 m_ViewportPanelPosition = { 0.0f, 0.0f };
			glm::vec2 m_ViewportPropSize = { 0.0f, 0.0f };

			int m_Gizmo = -1;


			bool m_IsViewportFocused;
			bool m_IsViewportHovered;


	};
}