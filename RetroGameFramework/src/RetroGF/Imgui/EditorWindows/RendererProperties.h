#pragma once


#include "RetroGF/Core.h"
#include "RetroGF/Layer.h"

#include "RetroGF/Events/WindowEvents.h"


namespace RGF {

	class RGF_API RendererProperties : public Layer {
		
		public :
			RendererProperties() : Layer("RendererProperties") {}
			~RendererProperties() {}


			virtual void OnUpdate() override {}
			virtual void OnEvent(Event& e) override {}

			virtual void Init() override;
			virtual void ShutDown() override;
			virtual void OnImguiRender() override;

	};

}