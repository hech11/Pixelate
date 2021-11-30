#pragma once

#include <Glm/glm.hpp>

#include <Pixelate/Core/Layer.h>

#include <Pixelate/Rendering/API/Texture.h>
#include <Pixelate/Rendering/ParticleSystem.h>
#include <Pixelate/Rendering/API/Framebuffer.h>

#include <Pixelate/Audio/AudioSource.h>

#include <Pixelate/Physics/RigidBody.h>

#include <Pixelate/Events/KeyEvents.h>
#include <Pixelate/Editor/EditorCamera.h>
#include <Pixelate/Editor/EditorSceneHierarchyPanel.h>
#include <Pixelate/Editor/EditorAnimationPanel.h>
#include <Pixelate/Editor/EditorConsoleLogger.h>
#include <Pixelate/Editor/EditorTextureInspector.h>
#include <Pixelate/Editor/EditorAudioPanel.h>
#include <Pixelate/Editor/EditorAudioMixerPanel.h>
#include <Pixelate/Editor/EditorContentBrowser.h>
#include <Pixelate/Editor/EditorPhysicsPropertiesPanel.h>
#include <Pixelate/Editor/EditorRendererPanel.h>
#include <Pixelate/Editor/EditorPanel.h>

#include <Pixelate/Scene/Scene.h>
#include <Pixelate/Scene/Components.h>
#include <Pixelate/Scene/Entity.h>

#include <Pixelate/Core/AABB.h>
#include <Pixelate/Animation/Animation.h>


namespace Pixelate {


	class EditorLayer : public Pixelate::Layer {
		public:
			virtual void Init() override;
			virtual void ShutDown() override;
	
			virtual void OnUpdate(float dt) override;
			virtual void OnEvent(Pixelate::Event& e) override;
		
			virtual void OnImguiRender() override;

		
		private: 
			bool OnKeyPressedEvent(KeyPressedEvent& e);
			bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

			void OnScenePlay();
			void OnSceneStop();

			void OpenScene();
			void SaveScene();

		private : 

			bool m_ShowStartupModal = true;

			enum class SceneState {
				None = -1,
				Edit,
				Play,
				Pause
			};

			int m_Gizmo = -1;
			bool m_OpeningModal = true; // temp
			bool m_OpenSceneManagerPanel = false;;

			SceneState m_SceneState = SceneState::Edit;


			// Scene Viewport panel 


			Ref<Framebuffer> m_SceneViewportFramebuffer;

			glm::vec2 m_SceneViewportSize = { 0.0f, 0.0f };
			glm::vec2 m_SceneViewportPanelSize = { 0.0f, 0.0f };
			glm::vec2 m_SceneViewportPanelPosition = { 0.0f, 0.0f };

			bool m_IsSceneViewportHovered;
			bool m_IsSceneViewportFocused;

			//

			// Game Viewport panel 

			Ref<Framebuffer> m_GameViewportFramebuffer;

			glm::vec2 m_GameViewportSize = { 0.0f, 0.0f };
			glm::vec2 m_GameViewportPanelSize = { 0.0f, 0.0f };
			glm::vec2 m_GameViewportPanelPosition = { 0.0f, 0.0f };

			bool m_IsGameViewportHovered;
			bool m_IsGameViewportFocused;

			//

// 
			Ref<EditorSceneHierarchyPanel> m_SceneHierarcyPanel;
			Ref<EditorCamera> m_EditorCamera;
			Ref<EditorConsoleLogger> m_ConsoleLog;
			Ref<EditorAnimationPanel> m_AnimatorPanel;
			Ref<EditorTextureInspector> m_TextureInspector;
			Ref<EditorAudioPanel> m_AudioPanel;
			Ref<EditorAudioMixerPanel> m_AudioMixerPanel;
			Ref<EditorContentBrowser> m_ContentBrowser;
			Ref<EditorPhysicsPropertiesPanel> m_PhysicsPanel;
			Ref<EditorRendererPanel> m_RendererPanel;


			ParticleSystem particleSystem;
			ParticleProperties particleProps;
			Ref<Scene> m_EditorScene;
		
			Animator anim;
			Entity animationTest;
			Entity animationComponentTest;
			Entity assetHandleTestEntity;
	};


}