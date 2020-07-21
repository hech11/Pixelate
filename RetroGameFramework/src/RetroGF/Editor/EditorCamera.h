#pragma once

#include "RetroGF/Events/Event.h"

#include "RetroGF/Rendering/OrthographicCamera.h"

#include "RetroGF/Events/MouseEvents.h"
#include "RetroGF/Events/WindowEvents.h"
#include "Glm/glm.hpp"

// TODO: This really should inherit orthographic camera. Do this.

namespace RGF{
	struct EditorViewportPanelData {
		glm::vec2 PanelPosition = { 0.0f, 0.0f };
		glm::vec2 PanelSize = { 0.0f, 0.0f };

	};
	class EditorCamera {
		public :

			EditorCamera(float aspectRatio, float ZoomLevel = 1);

			void OnUpdate(float ts, const EditorViewportPanelData& data);
			void OnEvent(Event& e);

			void Resize(float width, float height);
			
			const float& GetZoomLevel() const { return m_ZoomLevel; }

			OrthographicCamera& GetCamera() { return m_Camera; }
			const OrthographicCamera& GetCamera() const { return m_Camera; }
			const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }

			glm::vec2 GetMousePositionRelativeToViewportPanel();
			glm::vec2 GetMousePositionScreenSpace();

			bool IsIntersecting(const glm::vec3& position, const glm::vec3& scale); // this should take in a AABB
		private:
			bool OnMouseScrolled(MouseScrolledEvent& e);
			bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
			bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
			bool OnWindowResize(WindowResizeEvent& e);


		private : 
			bool m_Drag = false;
			bool m_MouseMoving = false;
			float m_AspectRatio = 0.0f;
			float m_ZoomLevel = 1.0f;
			float m_MouseSensitivity = 0.003f;

			float m_CameraTranslationSpeed = 3.0f;
			glm::vec2 m_CameraPosition = { 0.0f, 0.0f }, m_OriginalMousePosition = {0.0f, 0.0f};
			glm::vec2 m_OriginalCamPos = {0.0f, 0.0f};

			EditorViewportPanelData m_PanelData;

			OrthographicCamera m_Camera;
			OrthographicCameraBounds m_Bounds;


	};

}