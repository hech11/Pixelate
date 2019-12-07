#pragma once

#include "RetroGF/Core/Input.h"

// The windows version of input.

// Based on GLFW.
// TODO: Switch from GLFW's input to the "Windows.h" implementation of input.

namespace RGF {

	class RGF_API WindowsInput : public Input {

		bool IsKeyDownImpl(int keycode) override;

		bool IsMouseButtonDownImpl(int button) override;
		int GetMousePosXImpl() override;
		int GetMousePosYImpl() override;

	};

}