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
			ImguiEngineEditor();

			~ImguiEngineEditor() {}

			static ImguiEngineEditor& GetEditor() { return *s_Instance; }

			virtual void OnUpdate(float dt) override {}
			virtual void OnEvent(Event& e) override {}

			virtual void Init()override;
			virtual void ShutDown()override;
			virtual void OnImguiRender() override;

			void Start();
			void End();


			GameViewport& GetGameViewport() { return *m_GameView; }
			RendererProperties& GetRenderingProps() { return *m_RenderingProps; }
			ColorStyleEditor& GetStyleEditor() { return *m_EngineColEditor; }



		private :
			GameViewport* m_GameView;
			RendererProperties* m_RenderingProps;
			ColorStyleEditor* m_EngineColEditor;


			static ImguiEngineEditor* s_Instance;

	};

}
#endif