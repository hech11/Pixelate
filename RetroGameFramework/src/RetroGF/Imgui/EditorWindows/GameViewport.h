#pragma once
#ifndef RGF_DISTRIBUTE


#include "RetroGF/Core/Core.h"
#include "RetroGF/Core/Layer.h"

#include "RetroGF/Events/WindowEvents.h"
#include "RetroGF/Events/MouseEvents.h"
#include "RetroGF\Rendering\OrthographicCamera.h"

// This layer shows the game viewport.
// TODO: This uses layers. This could just be implemented into "ImguiEngineEdtior" without the use of layers. Investigate.


namespace RGF {

	class FrameBuffer;
	class RGF_API GameViewport : public Layer {

		public :
			GameViewport() : Layer("GameViewport"){}
			~GameViewport() {}




			virtual void OnUpdate(float dt) override;
			virtual void OnEvent(Event& e) override;

			virtual void Init()override;
			virtual void ShutDown()override;
			virtual void OnImguiRender() override;

			void Start();
			void End();


			Scoped<OrthographicCamera> GameViewCamera;
			FrameBuffer* ViewportFBO;
			unsigned int width, height;

		private :
			glm::vec2 m_GameviewWindowSize;
			glm::vec2 m_GameviewWindowPos;
			bool ZoomCamera(MouseScrolledEvent& e);
			bool MoveCamera(MouseMovedEvent& e);

			float m_ScaleSensitivity = 1.0f;

	};
}
#endif