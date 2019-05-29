#pragma once

#ifndef RGF_DISTRIBUTE

#include "RetroGF/Core.h"
#include "RetroGF/Layer.h"

#include "RetroGF/Events/WindowEvents.h"


// This allows to change Imgui's "Theme". This class can also create custom themes.
// (Although it's not really a main feature. Just used to develop the framework and used for fun)

// TODO: This uses layers. This could just be implemented into "ImguiEngineEdtior" without the use of layers. Investigate.

namespace RGF {

	class ColorStyleEditor : public Layer {

		public :
			ColorStyleEditor() : Layer("ColorStyleEditor") {}
			~ColorStyleEditor() {}
			virtual void OnUpdate() override {}
			virtual void OnEvent(Event& e) override {}

			virtual void Init() override;
			virtual void ShutDown() override;
			virtual void OnImguiRender() override;

			bool IsOpen = false;
	};

}

#endif