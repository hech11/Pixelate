#pragma once

/*

	Layers and overlays.
	They seperate graphical data and handle events in a stack like order. ( think photoshop layers )

	Overlays always execute last in the layer stack. Meaning they always render on top of layers.


	Example 
	{

		Layer1 -- "3d World" -- renders the 3d world.
		Layer2 -- "Post proccsesing effects" -- adds graphical effects to a scene.
		Layer3 -- "UI/HUD" -- The player's UI.

		Overlay1 -- "DebuggingOverlay" -- Might be useful to have always render on top of the game.

	}

*/


#include "RetroGF/Core.h"
#include "RetroGF/Events/Event.h"

namespace RGF {

	class RGF_API Layer {

		public : 
			Layer(const std::string& name = "Layer") : m_DebugName(name) { }
			virtual ~Layer() {ShutDown();}


			virtual void OnUpdate() {}
			virtual void OnEvent(Event& e) {}
			virtual void OnImguiRender() {}

			virtual void Init() {}
			virtual void ShutDown() {}

		protected :
			std::string m_DebugName; // TODO: Strip this in distrubute releases.

	};

}