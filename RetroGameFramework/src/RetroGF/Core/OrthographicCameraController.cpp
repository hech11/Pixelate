#include "RGFpch.h"
#include "OrthographicCameraController.h"

#include "RetroGF/Core/Input.h"
#include "RetroGF/Core/KeyCodes.h"

namespace RGF {

	OrthographicCameraController::OrthographicCameraController(float aspectRadio, bool rotation)
		: m_AspectRatio(aspectRadio),
		m_ZoomLevel(1.0f),
		m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
		m_Rotation(rotation),
		m_CameraPosition({ 0.0f, 0.0f, 0.0f }),
		m_CameraRotation(0.0f)
	{

	}

	void OrthographicCameraController::OnUpdate(float ts) {
		if (Input::IsKeyDown(RGF_KEY_A)) {
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts * m_ZoomLevel;
		}
		else if (Input::IsKeyDown(RGF_KEY_D)) {
			m_CameraPosition.x += m_CameraTranslationSpeed * ts * m_ZoomLevel;
		}

		if (Input::IsKeyDown(RGF_KEY_W)) {
			m_CameraPosition.y += m_CameraTranslationSpeed * ts * m_ZoomLevel;
		}
		else if (Input::IsKeyDown(RGF_KEY_S)) {
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts * m_ZoomLevel;
		}


		if (m_Rotation) {
			if (Input::IsKeyDown(RGF_KEY_Q)) {
				m_CameraRotation += m_CameraRotationSpeed * ts;
			}
			else if (Input::IsKeyDown(RGF_KEY_E)) {
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			}

			m_Camera.SetRotation(m_CameraRotation, 1.0f);

		}

		m_Camera.SetPosition(m_CameraPosition);
	}

	void OrthographicCameraController::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(RGF_BIND_EVENT_FNC(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(RGF_BIND_EVENT_FNC(OrthographicCameraController::OnWindowResize));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYScroll();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResize(WindowResizeEvent& e)
	{

		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}
