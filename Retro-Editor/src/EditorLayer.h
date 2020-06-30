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

	
	class EditorLayer : public RGF::Layer {
		public:
			virtual void Init() override;
			virtual void ShutDown() override;
	
			virtual void OnUpdate(float dt) override;
			virtual void OnEvent(RGF::Event& e) override;
		
			virtual void OnImguiRender() override;
		
		
			bool OnKeyPressedEvent(KeyPressedEvent& e);

		private :
			glm::vec3 SpritePosition, SpriteSize;
			float Rotation = 0.0f;
			glm::vec4 SpriteColor;

			Ref<Texture> LoadedFromFilepath, GeneratedTexture;
			Ref<TextureBounds> SmileySprite, GaspSprite;
			Ref<FrameBuffer> TestViewport;

			ParticleSystem particleSystem;
			ParticleProperties particleProps;

			Scoped<OrthographicCameraController> m_CameraController;
			Ref<AudioSource> toneSFX, sameToneSfx;
			Scoped<RigidBody> FloorRigidbody, PlayerRigidbody;
			float VelocitySpeed = 1;
	};


}