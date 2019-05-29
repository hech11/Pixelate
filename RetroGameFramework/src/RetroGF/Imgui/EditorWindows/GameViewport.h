#pragma once
#ifndef RGF_DISTRIBUTE


#include "RetroGF/Core.h"
#include "RetroGF/Layer.h"

#include "RetroGF/Events/WindowEvents.h"


// This layer shows the game viewport.
// TODO: This uses layers. This could just be implemented into "ImguiEngineEdtior" without the use of layers. Investigate.


namespace RGF {

	class FrameBuffer;
	class RGF_API GameViewport : public Layer {

		public :
			GameViewport() : Layer("GameViewport"){}
			~GameViewport() {}


			virtual void OnUpdate() override {}
			virtual void OnEvent(Event& e) override {}

			virtual void Init()override;
			virtual void ShutDown()override;
			virtual void OnImguiRender() override;

			void Start();
			void End();


			FrameBuffer* ViewportFBO;
	};
}
#endif