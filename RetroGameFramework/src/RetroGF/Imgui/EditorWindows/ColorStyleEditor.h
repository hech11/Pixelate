#pragma once


#include "RetroGF/Core.h"
#include "RetroGF/Layer.h"

#include "RetroGF/Events/WindowEvents.h"



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