#pragma once

#ifdef RGF_USE_IMGUI

#include "RetroGF/Core/Core.h"
#include "RetroGF/Core/Layer.h"

#include "RetroGF/Events/WindowEvents.h"

namespace RGF {

	class RGF_API ImguiLayer : public Layer {

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