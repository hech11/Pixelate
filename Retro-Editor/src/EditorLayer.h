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
		private : 
			Scoped<EditorViewportPanel> m_ViewportPanel;
			Ref<EditorSceneHierarchyPanel> m_SceneHierarcyPanel;

			ParticleSystem particleSystem;
			ParticleProperties particleProps;
			Ref<Scene> m_EditorScene;
			

	};


}