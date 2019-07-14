#pragma once

#ifndef RGF_DISTRIBUTE

#include "RetroGF/Core.h"
#include "RetroGF/Layer.h"

#include "RetroGF/Events/WindowEvents.h"



// This layer only initializes Imgui.
namespace RGF {

	// TODO: This may unnecessary as the "ImguiEngineEditor" could just initialize Imgui.
	class RGF_API ImguiLayer : public Layer {

		public :
			ImguiLayer() : Layer("Imgui Layer") {}
			~ImguiLayer() {}


			virtual void OnUpdate(float dt) override {}
			virtual void OnEvent(Event& e) override{}

			virtual void Init()override;
			virtual void ShutDown()override;
			virtual void OnImguiRender() override;

			void Start();
			void End();

	};

}
#endif