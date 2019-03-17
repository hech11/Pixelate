#pragma once

#ifndef RGF_DISTRIBUTE

#include "RetroGF/Core.h"
#include "RetroGF/Layer.h"

#include "RetroGF/Events/WindowEvents.h"


namespace RGF {

	class RGF_API ImguiLayer : public Layer {

		public :
			ImguiLayer() : Layer("Imgui Layer") {}
			~ImguiLayer() {}


			virtual void OnUpdate() override {}
			virtual void OnEvent(Event& e) override{}

			virtual void Init()override;
			virtual void ShutDown()override;
			virtual void OnImguiRender() override;

			void Start();
			void End();

	};

}
#endif