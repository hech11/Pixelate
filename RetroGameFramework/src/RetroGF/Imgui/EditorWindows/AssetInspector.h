#pragma once



#ifndef RGF_DISTRIBUTE

#include "RetroGF/Core.h"
#include "RetroGF/Layer.h"

#include "RetroGF/Events/WindowEvents.h"

// TODO: This uses layers. This could just be implemented into "ImguiEngineEdtior" without the use of layers. Investigate.

namespace RGF {
	class Sprite;
	class AssetInspector : public Layer {

		public :
			AssetInspector() : Layer("ColorStyleEditor") {}
			~AssetInspector() {}
			virtual void OnUpdate(float dt) override {}
			virtual void OnEvent(Event& e) override {}

			virtual void Init() override;
			virtual void ShutDown() override;
			virtual void OnImguiRender() override;

			void PushSprite(Sprite* sprite);

		private :
			//TODO: This is temp
			std::vector<Sprite*> m_Sprites;
	};

}

#endif