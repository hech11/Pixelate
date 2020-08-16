#include "PXpch.h"

#include "Pixelate/Core/Input.h"

#include "Pixelate/Core/Application.h"
#include "GLFW/include/GLFW/glfw3.h"


namespace Pixelate {
	Input::MouseLockMode Input::m_LockmodeState;


	bool Input::IsKeyDown(KeyCode keycode) {
		auto win = static_cast<GLFWwindow*>(Application::GetApp().GetWindow().GetNativeWindow());

		int state = glfwGetKey(win, (int)keycode);
		return state == GLFW_PRESS;
	}

	bool Input::IsMouseButtonDown(MouseButton button) {
		auto win = static_cast<GLFWwindow*>(Application::GetApp().GetWindow().GetNativeWindow());

		int state = glfwGetMouseButton(win, (int)button);
		return state == GLFW_PRESS;
	}
	int Input::GetMousePosX() {
		auto win = static_cast<GLFWwindow*>(Application::GetApp().GetWindow().GetNativeWindow());

		double xpos;
		glfwGetCursorPos(win, &xpos, nullptr);
		return xpos;
	}
	int Input::GetMousePosY() {
		auto win = static_cast<GLFWwindow*>(Application::GetApp().GetWindow().GetNativeWindow());

		double ypos;
		glfwGetCursorPos(win, nullptr, &ypos);
		return ypos;
	}


	std::pair<int, int> Input::GetMousePos() {
		auto win = static_cast<GLFWwindow*>(Application::GetApp().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(win, &xpos, &ypos);
		return std::make_pair((int)xpos, (int)ypos);
	}


	void Input::SetMouseLockMode(Input::MouseLockMode lockMode) {
		m_LockmodeState = lockMode;
		auto win = static_cast<GLFWwindow*>(Application::GetApp().GetWindow().GetNativeWindow());
		if (m_LockmodeState == MouseLockMode::None) {
			glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		} else {
			glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}


}