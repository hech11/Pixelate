#pragma once

#include <Glm/glm.hpp>

#include <RetroGF/Core/Layer.h>

#include <RetroGF/Rendering/API/Texture.h>
#include <RetroGF/Rendering/ParticleSystem.h>
#include <RetroGF/Rendering/API/FrameBuffer.h>

#include <RetroGF/Audio/AudioSource.h>

#include <RetroGF/Physics/RigidBody.h>

#include <RetroGF/Events/KeyEvents.h>
#include <RetroGF/Editor/EditorCamera.h>
#include <RetroGF/Editor/EditorSceneHierarchyPanel.h>

#include <RetroGF/Scene/Scene.h>
#include <RetroGF/Scene/Components.h>
#include <RetroGF/Scene/Entity.h>

#include <RetroGF/Core/AABB.h>

namespace RGF {

	
	class EditorLayer : public RGF::Layer {
		public:
			virtual void Init() override;
			virtual void ShutDown() override;
	
			virtual void OnUpdate(float dt) override;
			virtual void OnEvent(RGF::Event& e) override;
		
			virtual void OnImguiRender() override;
		
		private: 
			bool OnKeyPressedEvent(KeyPressedEvent& e);
			bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

			void OnScenePlay();
			void OnSceneStop();
		private : 

			enum class SceneState {
				None = -1,
				Edit,
				Play,
				Pause
			};

			int m_Gizmo = -1;

			SceneState m_SceneState = SceneState::Edit;


			// Scene Viewport panel 


			Ref<FrameBuffer> m_SceneViewportFramebuffer;

			glm::vec2 m_SceneViewportSize = { 0.0f, 0.0f };
			glm::vec2 m_SceneViewportPanelSize = { 0.0f, 0.0f };
			glm::vec2 m_SceneViewportPanelPosition = { 0.0f, 0.0f };

			bool m_IsSceneViewportHovered;
			bool m_IsSceneViewportFocused;

			//

			// Game Viewport panel 

			Ref<FrameBuffer> m_GameViewportFramebuffer;

			glm::vec2 m_GameViewportSize = { 0.0f, 0.0f };
			glm::vec2 m_GameViewportPanelSize = { 0.0f, 0.0f };
			glm::vec2 m_GameViewportPanelPosition = { 0.0f, 0.0f };

			bool m_IsGameViewportHovered;
			bool m_IsGameViewportFocused;

			//


			Ref<EditorSceneHierarchyPanel> m_SceneHierarcyPanel;
			Ref<EditorCamera> m_EditorCamera;

			ParticleSystem particleSystem;
			ParticleProperties particleProps;
			Ref<Scene> m_EditorScene;
			

	};


}