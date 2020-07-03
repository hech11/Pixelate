#pragma once

#include "RetroGF/Core/Core.h"


// This represents the users input.
// This interface will be inherited by the platform specific classes such as "WindowsInput".


// Only supports Windows!
// This supports keyboard and mouse.

// TODO: Does not support gamepad. Look at gamepad support?
// TODO: This only gets the mouse and key button down. Add a "MouseButtonHeld" or "MouseButtonReleased" function? -- ( same with keyboard )
namespace RGF {

	class RGF_API Input {

		public :

			static bool IsKeyDown(int keycode);

			static bool IsMouseButtonDown(int button);
			static int GetMousePosX();
			static int GetMousePosY();
			static std::pair<int, int> GetMousePos();


	};

}