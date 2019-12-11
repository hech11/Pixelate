#pragma once



#ifndef RGF_DISTRIBUTE

#include "RetroGF/Core/Core.h"
#include "RetroGF/Core/Layer.h"

#include "RetroGF/Events/WindowEvents.h"

// TODO: This uses layers. This could just be implemented into "ImguiEngineEdtior" without the use of layers. Investigate.

namespace RGF {
	class Sprite;
	class SpriteInspector : public Layer {

		public :
			SpriteInspector() : Layer("ColorStyleEditor") {}
			~SpriteInspector() {}
			virtual void OnUpdate(float dt) override {}
			virtual void OnEvent(Event& e) override {}

			virtual void Init() override;
			virtual void ShutDown() override;
			virtual void OnImguiRender() override;

			void PushSprite(const Ref<Sprite>& sprite);

		private :
			//TODO: This is temp
			std::vector<Ref<Sprite>> m_Sprites;
	};

}

#endif