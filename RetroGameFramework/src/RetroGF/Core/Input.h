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

			virtual ~Input() {
				if (s_Instance != nullptr)
					delete s_Instance;
			}
			static inline bool IsKeyDown(int keycode) { return s_Instance->IsKeyDownImpl(keycode); };

			static inline bool IsMouseButtonDown(int button) { return s_Instance->IsMouseButtonDownImpl(button); }
			static inline int GetMousePosX() { return s_Instance->GetMousePosXImpl(); }
			static inline int GetMousePosY() { return s_Instance->GetMousePosYImpl(); }

		protected :
			virtual bool IsKeyDownImpl(int keycode) = 0;

			virtual bool IsMouseButtonDownImpl(int button) = 0;
			virtual int GetMousePosXImpl() = 0;
			virtual int GetMousePosYImpl() = 0;


		private : 
			static Input* s_Instance;
	};

}