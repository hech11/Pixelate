#pragma once

#include "Pixelate/Core/Core.h"


// This represents the users input.
// This interface will be inherited by the platform specific classes such as "WindowsInput".


// Only supports Windows!
// This supports keyboard and mouse.

// TODO: Does not support gamepad. Look at gamepad support?
// TODO: This only gets the mouse and key button down. Add a "MouseButtonHeld" or "MouseButtonReleased" function? -- ( same with keyboard )
namespace Pixelate {

	class PX_API Input {

		public :
			enum class MouseLockMode {
				None = 0,
				Locked
			};
			static bool IsKeyDown(int keycode);

			static bool IsMouseButtonDown(int button);
			static int GetMousePosX();
			static int GetMousePosY();
			static std::pair<int, int> GetMousePos();
			static void SetMouseLockMode(MouseLockMode lockMode);
		private :
			static MouseLockMode m_LockmodeState;
	};

}