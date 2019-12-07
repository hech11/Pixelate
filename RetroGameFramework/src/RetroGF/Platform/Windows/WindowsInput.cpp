#include "RGFpch.h"
#include "WindowsInput.h"

#include "RetroGF/Core/Application.h"
#include "GLFW/include/GLFW/glfw3.h"

namespace RGF {

	// TODO: This needs to be "macro-fied" as this input system is only hardcoded to support windows.
	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyDownImpl(int keycode) {
		auto win = static_cast<GLFWwindow*>(Application::GetApp().GetWindow().GetNativeWindow());

		int state = glfwGetKey(win, keycode);
		return state == GLFW_PRESS;
	}

	bool WindowsInput::IsMouseButtonDownImpl(int button) {
		auto win = static_cast<GLFWwindow*>(Application::GetApp().GetWindow().GetNativeWindow());

		int state = glfwGetMouseButton(win, button);
		return state == GLFW_PRESS;
	}
	int WindowsInput::GetMousePosXImpl() {
		auto win = static_cast<GLFWwindow*>(Application::GetApp().GetWindow().GetNativeWindow());

		double xpos;
		glfwGetCursorPos(win, &xpos, nullptr);
		return xpos;
	}
	int WindowsInput::GetMousePosYImpl() {
		auto win = static_cast<GLFWwindow*>(Application::GetApp().GetWindow().GetNativeWindow());

		double ypos;
		glfwGetCursorPos(win, nullptr, &ypos);
		return ypos;
	}

}