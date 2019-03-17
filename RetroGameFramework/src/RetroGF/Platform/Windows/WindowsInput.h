#pragma once

#include "RetroGF/InputImpl.h"

namespace RGF {

	class RGF_API WindowsInput : public InputImpl {

		bool IsKeyDownImpl(int keycode) override;

		bool IsMouseButtonDownImpl(int button) override;
		int GetMousePosXImpl() override;
		int GetMousePosYImpl() override;

	};

}