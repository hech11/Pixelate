#pragma once

#include <Glm/glm.hpp>

#include <RetroGF/Core/OrthographicCameraController.h>
#include <RetroGF/Core/Layer.h>

#include <RetroGF/Rendering/API/Texture.h>
#include <RetroGF/Rendering/ParticleSystem.h>
#include <RetroGF/Rendering/API/FrameBuffer.h>

#include <RetroGF/Audio/AudioSource.h>

#include <RetroGF/Physics/RigidBody.h>

#include <RetroGF/Events/KeyEvents.h>


namespace RGF {

	
// A example layer.
	class EditorLayer : public RGF::Layer {
		public:
			virtual void Init() override;
			virtual void ShutDown() override;
	
			virtual void OnUpdate(float dt) override;
			virtual void OnEvent(RGF::Event& e) override;
		
			virtual void OnImguiRender() override;
		
		
			bool OnKeyPressedEvent(RGF::KeyPressedEvent& e);

		private :
			glm::vec3 SpritePosition, SpriteSize;
			float Rotation = 0.0f;
			glm::vec4 SpriteColor;

			RGF::Ref<RGF::Texture> LoadedFromFilepath, GeneratedTexture;
			RGF::Ref<RGF::TextureBounds> SmileySprite, GaspSprite;
			//	RGF::Ref<RGF::FrameBuffer> TestViewport;

			RGF::ParticleSystem particleSystem;
			RGF::ParticleProperties particleProps;

			RGF::Scoped<RGF::OrthographicCameraController> m_CameraController;
			RGF::Ref<RGF::AudioSource> toneSFX, sameToneSfx;
			RGF::Scoped<RGF::RigidBody> FloorRigidbody, PlayerRigidbody;
			float VelocitySpeed = 1;
	};


}