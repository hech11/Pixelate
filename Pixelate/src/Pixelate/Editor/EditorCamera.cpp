#include "PXpch.h"
#include "EditorCamera.h"
#include "../Core/Input.h"
#include <Pixelate\Debug\Instrumentor.h>
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
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
namespace Pixelate {



	EditorCamera::EditorCamera(float aspectRatio, const ViewportPanelProps& props, float orhographicScale) :
		OrthographicCamera({-aspectRatio* orhographicScale, aspectRatio* orhographicScale, -orhographicScale, orhographicScale })
	{
		m_ViewportProps = props;
		Resize(m_ViewportProps.Size->x / m_ViewportProps.Size->y);
	}

	void EditorCamera::OnUpdate(float ts) {
// 		auto mViewspace = GetMousePositionViewportSpace();
// 		PX_CORE_WARN("%f, %f\n", mViewspace.x, mViewspace.y);

		if (m_Drag) {
			auto getCurrentMousePos = GetMousePositionRelativeToViewportPanel();
			glm::vec2 posDelta;
			posDelta.x = m_OriginalMousePosition.x - getCurrentMousePos.x;
			posDelta.y = getCurrentMousePos.y - m_OriginalMousePosition.y; // This inverts the y movement


			glm::vec2 m_CameraPosition = (posDelta * m_OrthographicSize * m_MouseSensitivity + m_OriginalCamPos);
			SetPosition({ m_CameraPosition.x, m_CameraPosition.y, 0.0f });
		}
		else {
			const bool& IsAltHeld = (Input::IsKeyDown(KeyCode::LeftAlt) || Input::IsKeyDown(KeyCode::Right));
			if (Input::IsKeyDown(KeyCode::A) && IsAltHeld) {

				Move({ -m_CameraTranslationSpeed * ts * m_OrthographicSize, 0.0f, 0.0f});
			}
			else if (Input::IsKeyDown(KeyCode::D) && IsAltHeld) {
				Move({ m_CameraTranslationSpeed * ts * m_OrthographicSize, 0.0f, 0.0f });
			}

			if (Input::IsKeyDown(KeyCode::W) && IsAltHeld) {
				Move({ 0.0f, m_CameraTranslationSpeed * ts * m_OrthographicSize, 0.0f });
			}
			else if (Input::IsKeyDown(KeyCode::S) && IsAltHeld) {
				Move({ 0.0f, -m_CameraTranslationSpeed * ts * m_OrthographicSize, 0.0f });
			}
		}


	}



	void EditorCamera::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(PX_BIND_EVENT_FNC(EditorCamera::OnMouseScrolled));
		dispatcher.Dispatch<MouseButtonPressedEvent>(PX_BIND_EVENT_FNC(EditorCamera::OnMouseButtonPressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(PX_BIND_EVENT_FNC(EditorCamera::OnMouseButtonReleased));
		dispatcher.Dispatch<WindowResizeEvent>(PX_BIND_EVENT_FNC(EditorCamera::OnWindowResize));
	}


	bool EditorCamera::IsIntersecting(const glm::vec3& position, const glm::vec3& scale) {
		// Not ideal...

		// Convert mouse position from pixel space to world space
		auto mouse = GetMousePositionRelativeToViewportPanel();
		glm::vec3 mousepos = glm::vec3(mouse.x, mouse.y, 1.0f);
		glm::vec4 viewport = glm::vec4(0.0f, 0.0f, m_ViewportProps.Size->x, m_ViewportProps.Size->y);
		glm::vec3 xMouseWorld = glm::unProject(mousepos, GetViewMatrix(), GetProjectionMatrix(), viewport);
		glm::vec3 yMouseWorld = glm::unProject(mousepos, glm::inverse(GetViewMatrix()), GetProjectionMatrix(), viewport);

		glm::vec3 result = { xMouseWorld.x, yMouseWorld.y, 1.0f };
		result.y *= -1.0f;


		// Check for intersection
		bool collisionX = result.x >= position.x - (scale.x / 2.0f) && result.x <= position.x + (scale.x / 2.0f);
		bool collisionY = result.y >= position.y - (scale.y / 2.0f) && result.y <= position.y + (scale.y / 2.0f);

		return collisionX && collisionY;
	}


	bool EditorCamera::OnMouseScrolled(MouseScrolledEvent& e) {
		m_OrthographicSize -= e.GetYScroll() * 0.25f;
		m_OrthographicSize = std::max(m_OrthographicSize, 0.25f);

		m_Bounds = { -m_AspectRatio * m_OrthographicSize, m_AspectRatio * m_OrthographicSize, -m_OrthographicSize, m_OrthographicSize };
		SetProjection(m_Bounds);

		return false;
	}


	bool EditorCamera::OnMouseButtonPressed(MouseButtonPressedEvent& e) {
		if (e.GetButton() == (int)MouseButton::Right && e.GetRepeatCount() == 0) {
			auto& app = Application::GetApp();

			m_OriginalMousePosition = GetMousePositionRelativeToViewportPanel();
			m_OriginalCamPos = GetPos();
			//Input::SetMouseLockMode(Input::MouseLockMode::Locked);

			m_Drag = true;
		}

		return false;
	}

	bool EditorCamera::OnMouseButtonReleased(MouseButtonReleasedEvent& e) {

		if (e.GetButton() == (int)MouseButton::Right) {
			m_Drag = false;
			//Input::SetMouseLockMode(Input::MouseLockMode::None);

		}

		return false;
	}


	bool EditorCamera::OnWindowResize(WindowResizeEvent& e) {
		Resize((float)e.GetWidth() /  (float)e.GetHeight());
		return false;
	}


	glm::vec2 EditorCamera::GetMousePositionRelativeToViewportPanel() {
		auto& app = Application::GetApp();

		glm::vec2 viewportPanelPosRelativeToWindow;
		

		viewportPanelPosRelativeToWindow.x = m_ViewportProps.Position->x - app.GetWindow().GetXPos();
		viewportPanelPosRelativeToWindow.y = m_ViewportProps.Position->y - app.GetWindow().GetYPos();
		glm::vec2 mousePositionRelativeToWindow = { Input::GetMousePosX(), Input::GetMousePosY() };

		return mousePositionRelativeToWindow - viewportPanelPosRelativeToWindow;

	}

	glm::vec2 EditorCamera::GetMousePositionScreenSpace() {
		auto& app = Application::GetApp();


		auto mPos = GetMousePositionRelativeToViewportPanel();
		auto& panelSize = m_ViewportProps.Size;
		
		glm::vec2 result;
		result.x = (2.0f * mPos.x) / panelSize->x - 1.0f;
		result.y = (2.0f * mPos.y) / panelSize->y - 1.0f;

		return result;
	}


}