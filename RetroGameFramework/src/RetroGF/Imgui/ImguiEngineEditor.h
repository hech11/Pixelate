#pragma once

#ifndef RGF_DISTRIBUTE

#include "RetroGF/Core.h"
#include "RetroGF/Layer.h"

#include "RetroGF/Events/WindowEvents.h"

#include "EditorWindows/GameViewport.h"
#include "EditorWindows/RendererProperties.h"
#include "EditorWindows/ColorStyleEditor.h"

namespace RGF {

	class FrameBuffer;
	class RGF_API ImguiEngineEditor : public Layer {

		public :
			ImguiEngineEditor() : Layer("Imgui Layer") {}
			~ImguiEngineEditor() {}


			virtual void OnUpdate(float dt) override {}
			virtual void OnEvent(Event& e) override {}

			virtual void Init()override;
			virtual void ShutDown()override;
			virtual void OnImguiRender() override;

			void Start();
			void End();

			GameViewport* GameView;
			RendererProperties* RenderingProps;
			ColorStyleEditor* EngineColEditor;
	};

}
#endif