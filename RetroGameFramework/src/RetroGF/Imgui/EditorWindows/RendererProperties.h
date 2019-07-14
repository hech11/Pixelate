#pragma once
#ifndef RGF_DISTRIBUTE


#include "RetroGF/Core.h"
#include "RetroGF/Layer.h"

#include "RetroGF/Events/WindowEvents.h"


// Renders everything to do with the Renderer2D via an imgui window
// TODO: This uses layers. This could just be implemented into "ImguiEngineEdtior" without the use of layers. Investigate.

namespace RGF {

	class RGF_API RendererProperties : public Layer {
		
		public :
			RendererProperties() : Layer("RendererProperties") {}
			~RendererProperties() {}


			virtual void OnUpdate(float dt) override {}
			virtual void OnEvent(Event& e) override {}

			virtual void Init() override;
			virtual void ShutDown() override;
			virtual void OnImguiRender() override;

	};

}

#endif