#pragma once

#if 0

#include "RetroGF/Core/Core.h"
#include "RetroGF/Core/Layer.h"

#include "RetroGF/Events/WindowEvents.h"

#include "EditorWindows/GameViewport.h"
#include "EditorWindows/RendererProperties.h"
#include "EditorWindows/ColorStyleEditor.h"
#include "EditorWindows/SpriteInspector.h"

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
			SpriteInspector& GetAssetInspector() { return *m_AssetInspector; }


		private :
			GameViewport* m_GameView;
			RendererProperties* m_RenderingProps;
			ColorStyleEditor* m_EngineColEditor;
			SpriteInspector* m_AssetInspector;


			static ImguiEngineEditor* s_Instance;

	};

}
#endif