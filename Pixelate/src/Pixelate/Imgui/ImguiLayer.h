#pragma once

#ifdef PX_USE_IMGUI

#include "Pixelate/Core/Core.h"
#include "Pixelate/Core/Layer.h"

#include "Pixelate/Events/WindowEvents.h"

namespace Pixelate {

	class PX_API ImguiLayer : public Layer {

		public:
			ImguiLayer();

			~ImguiLayer() {}


			virtual void OnUpdate(float dt) override {}
			virtual void OnEvent(Event& e) override;


			virtual void Init()override;
			virtual void ShutDown()override;
			virtual void OnImguiRender() override;

			void ShouldBlockEvents(bool block) { m_BlockEvents = block; }

			void Start();
			void End();

		private :
			bool m_BlockEvents;

	};

}

#endif