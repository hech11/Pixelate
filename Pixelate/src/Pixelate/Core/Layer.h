#pragma once

/*

	Layers and overlays.
	They separate graphical data and handle events in a stack like order. ( think photoshop layers )

	Overlays always execute last in the layer stack. Meaning they always render on top of layers.


	Example 
	{

		Layer1 -- "3d World" -- renders the 3d world.
		Layer2 -- "Post possessing effects" -- adds graphical effects to a scene.
		Layer3 -- "UI/HUD" -- The player's UI.

		Overlay1 -- "DebuggingOverlay" -- Might be useful to have always render on top of the game.

	}

*/


#include "Pixelate/Core/Core.h"
#include "Pixelate/Events/Event.h"

namespace Pixelate {

	class PX_API Layer {

		public : 
			Layer(const std::string& name = "Layer") : m_DebugName(name) { }
			virtual ~Layer() {ShutDown();}


			virtual void OnUpdate(float dt) {}
			virtual void OnEvent(Event& e) {}
			virtual void OnImguiRender() {}

			virtual void Init() {}
			virtual void ShutDown() {}

			virtual std::string& GetName() { return m_DebugName; }

		protected :
			std::string m_DebugName; // TODO: Strip this in distrubute releases.

	};

}