#pragma once

#include "RetroGF/Rendering/OrthographicCamera.h"

#include "RetroGF/Events/MouseEvents.h"
#include "RetroGF/Events/WindowEvents.h"


namespace RGF {

	class OrthographicCameraController {
		public :
			OrthographicCameraController(float aspectRadio, bool rotation = false);


			void OnUpdate(float ts); // TODO: Create a timestep class!
			void OnEvent(Event& e);

			OrthographicCamera& GetCamera() { return m_Camera; }
			const OrthographicCamera& GetCamera() const { return m_Camera; }



		private :
			bool OnMouseScrolled(MouseScrolledEvent& e);
			bool OnWindowResize(WindowResizeEvent& e);

		private :
			float m_AspectRatio, m_ZoomLevel = 1.0f;
			OrthographicCamera m_Camera;

			bool m_Rotation = false;
			glm::vec3 m_CameraPosition;
			float m_CameraRotation;
			float m_CameraTranslationSpeed = 3.0f, m_CameraRotationSpeed = 3.0f;
	};

}
