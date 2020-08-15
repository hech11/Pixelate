#pragma once

#include "Pixelate/Events/Event.h"

#include "Pixelate/Rendering/OrthographicCamera.h"

#include "Pixelate/Events/MouseEvents.h"
#include "Pixelate/Events/WindowEvents.h"
#include "Glm/glm.hpp"



namespace Pixelate {
	
	struct ViewportPanelProps {
		glm::vec2* Position;
		glm::vec2* Size;
	};

	class EditorCamera : public OrthographicCamera {
		public :

			EditorCamera(float aspectRatio, const ViewportPanelProps& props, float orhographicScale = 1.0f);

			void OnUpdate(float ts);
			void OnEvent(Event& e);


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
			float m_MouseSensitivity = 0.003f;

			float m_CameraTranslationSpeed = 3.0f;
			glm::vec2 m_OriginalMousePosition = {0.0f, 0.0f};
			glm::vec2 m_OriginalCamPos = {0.0f, 0.0f};

			ViewportPanelProps m_ViewportProps;


	};

}