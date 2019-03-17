#pragma once

#include "RetroGF/Core.h"

namespace RGF {

	class RGF_API InputImpl {

		public :


			virtual ~InputImpl() {
				if (s_Instance != nullptr)
					delete s_Instance;
			}
			static bool IsKeyDown(int keycode) { return s_Instance->IsKeyDownImpl(keycode); };

			static bool IsMouseButtonDown(int button) { return s_Instance->IsMouseButtonDown(button); }
			static int GetMousePosX() { return s_Instance->GetMousePosX(); }
			static int GetMousePosY() { return s_Instance->GetMousePosY(); }

		protected :
			virtual bool IsKeyDownImpl(int keycode) = 0;

			virtual bool IsMouseButtonDownImpl(int button) = 0;
			virtual int GetMousePosXImpl() = 0;
			virtual int GetMousePosYImpl() = 0;


		private : 
			static InputImpl* s_Instance;
	};

}