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
#include <RetroGF/Editor/EditorViewportPanel.h>
#include <RetroGF/Editor/EditorSceneHierarchyPanel.h>

#include <RetroGF/Scene/Scene.h>
#include <RetroGF/Scene/Components.h>
#include <RetroGF/Scene/Entity.h>


namespace RGF {

	
	class EditorLayer : public RGF::Layer {
		public:
			virtual void Init() override;
			virtual void ShutDown() override;
	
			virtual void OnUpdate(float dt) override;
			virtual void OnEvent(RGF::Event& e) override;
		
			virtual void OnImguiRender() override;
		
		
			bool OnKeyPressedEvent(KeyPressedEvent& e);
		private : 
			Scoped<EditorViewportPanel> m_ViewportPanel;
			Scoped<EditorSceneHierarchyPanel> m_SceneHierarcyPanel;

			glm::vec3 SpritePosition, SpriteSize;
			float Rotation = 0.0f;
			glm::vec4 SpriteColor;

			Ref<Texture> LoadedFromFilepath, GeneratedTexture;
			Ref<TextureBounds> SmileySprite, GaspSprite;
			Ref<FrameBuffer> TestViewport;

			ParticleSystem particleSystem;
			ParticleProperties particleProps;

			Ref<AudioSource> toneSFX, sameToneSfx;
			Scoped<RigidBody> FloorRigidbody, PlayerRigidbody;
			float VelocitySpeed = 1;

			Ref<Scene> m_CurrentScene;
			Ref<Entity> m_TestEntity;
	};


}