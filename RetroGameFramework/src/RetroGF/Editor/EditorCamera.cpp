#include "RGFpch.h"
#include "EditorCamera.h"
#include "../Core/Input.h"
#include <RetroGF\Debug\Instrumentor.h>
#include "../Core/Core.h"
#include "../Core/KeyCodes.h"
#include "../Core/MouseButtonCodes.h"



#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/ext/matrix_projection.hpp"


#include <algorithm>
#include "glm/gtx/dual_quaternion.hpp"

#include <GLM/glm/gtc/constants.hpp>
#define GLM_ENABLE_EXERIMENTAL
#include <GLM/glm/gtx/compatibility.hpp>
#include "../Core/Application.h"

namespace RGF {



	EditorCamera::EditorCamera(float aspectRatio, float ZoomLevel) :
		m_AspectRatio(aspectRatio),
		m_ZoomLevel(ZoomLevel),
		m_Bounds({ -aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }),
		m_Camera(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top)
	{
	}

	void EditorCamera::OnUpdate(float ts, const EditorViewportPanelData& data) {

		m_PanelData = data;
		if (m_Drag) {
			auto getCurrentMousePos = GetMousePositionRelativeToViewportPanel();

			glm::vec2 posDelta;
			posDelta.x = m_OriginalMousePosition.x - getCurrentMousePos.x;
			posDelta.y = getCurrentMousePos.y - m_OriginalMousePosition.y; // This inverts the y movement


			m_CameraPosition = (posDelta * m_ZoomLevel * m_MouseSensitivity + m_OriginalCamPos);
			m_Camera.SetPosition({ m_CameraPosition.x, m_CameraPosition.y, 0.0f });
		}
		else {
			const bool& IsAltHeld = (Input::IsKeyDown(RGF_KEY_LEFT_ALT) || Input::IsKeyDown(RGF_KEY_RIGHT));
			if (Input::IsKeyDown(RGF_KEY_A) && IsAltHeld) {

				m_CameraPosition.x -= m_CameraTranslationSpeed * ts * m_ZoomLevel;
			}
			else if (Input::IsKeyDown(RGF_KEY_D) && IsAltHeld) {
				m_CameraPosition.x += m_CameraTranslationSpeed * ts * m_ZoomLevel;
			}

			if (Input::IsKeyDown(RGF_KEY_W) && IsAltHeld) {
				m_CameraPosition.y += m_CameraTranslationSpeed * ts * m_ZoomLevel;
			}
			else if (Input::IsKeyDown(RGF_KEY_S) && IsAltHeld) {
				m_CameraPosition.y -= m_CameraTranslationSpeed * ts * m_ZoomLevel;
			}
			m_Camera.SetPosition({ m_CameraPosition.x, m_CameraPosition.y, 0.0f });
		}




	}

	void EditorCamera::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(RGF_BIND_EVENT_FNC(EditorCamera::OnMouseScrolled));
		dispatcher.Dispatch<MouseButtonPressedEvent>(RGF_BIND_EVENT_FNC(EditorCamera::OnMouseButtonPressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(RGF_BIND_EVENT_FNC(EditorCamera::OnMouseButtonReleased));
		dispatcher.Dispatch<WindowResizeEvent>(RGF_BIND_EVENT_FNC(EditorCamera::OnWindowResize));
	}


	void EditorCamera::Resize(float width, float height)
	{
		m_AspectRatio = width / height;
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	bool EditorCamera::OnMouseScrolled(MouseScrolledEvent& e) {
		m_ZoomLevel -= e.GetYScroll() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);

		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}


	bool EditorCamera::OnMouseButtonPressed(MouseButtonPressedEvent& e) {
		if (e.GetButton() == RGF_MOUSE_BUTTON_RIGHT && e.GetRepeatCount() == 0) {
			auto& app = Application::GetApp();

			m_OriginalMousePosition = GetMousePositionRelativeToViewportPanel();
			m_OriginalCamPos = m_Camera.GetPos();

			m_Drag = true;
		}

		return false;
	}

	bool EditorCamera::OnMouseButtonReleased(MouseButtonReleasedEvent& e) {

		if (e.GetButton() == RGF_MOUSE_BUTTON_RIGHT) {
			m_Drag = false;
		}

		return false;
	}

	bool EditorCamera::OnWindowResize(WindowResizeEvent& e) {
		Resize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}


	glm::vec2 EditorCamera::GetMousePositionRelativeToViewportPanel() {
		auto& app = Application::GetApp();

		glm::vec2 viewportPanelPosRelativeToWindow;
		viewportPanelPosRelativeToWindow.x = m_PanelData.PanelPosition.x - app.GetWindow().GetXPos();
		viewportPanelPosRelativeToWindow.y = m_PanelData.PanelPosition.y - app.GetWindow().GetYPos();
		glm::vec2 mousePositionRelativeToWindow = { Input::GetMousePosX(), Input::GetMousePosY() };

		return mousePositionRelativeToWindow - viewportPanelPosRelativeToWindow;

	}


}