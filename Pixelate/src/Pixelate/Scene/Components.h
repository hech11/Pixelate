#pragma once

#include "Pixelate/Rendering/OrthographicCamera.h"
#include "Pixelate/Rendering/API/Texture.h"
#include "glm/glm.hpp"
#include "Pixelate/Core/AABB.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/matrix_decompose.hpp"

#include "Pixelate/Physics/RigidBody.h"
#include "Pixelate/Scripting/ScriptingMaster.h"

#include "Pixelate/Audio/AudioListener.h"
#include "Pixelate/Audio/AudioSource.h"
#include "Pixelate/Core/UUID.h"
#include "Pixelate/Core/Rect.h"

#include "Pixelate/Animation/Animation.h"

namespace Pixelate {
	
	

///////////////////////////////////////////////////// Misc components /////////////////////////////////////////////////////

	struct UUIDComponent {
		Pixelate::UUID UUID;

		UUIDComponent() = default;
		UUIDComponent(const UUIDComponent& other)
			: UUID(other.UUID) {}
		UUIDComponent(const Pixelate::UUID& uuid)
			: UUID(uuid) {}
	};

	struct TagComponent {
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent& other)
			: Tag(other.Tag) {}
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct NameComponent
	{
		std::string Name;

		NameComponent() = default;
		NameComponent(const NameComponent& other)
			: Name(other.Name) {}
		NameComponent(const std::string& name)
			: Name(name) {}


		
	};


	struct TransformComponent {
		glm::mat4 Transform = glm::mat4(1.0f);

		TransformComponent() = default;

		TransformComponent(const TransformComponent& other) :
			Transform(other.Transform)
		{ 
		}
		TransformComponent(const glm::mat4& transform)
			: Transform(transform)
		{
		}

		std::tuple<glm::vec3, glm::quat, glm::vec3> DecomposeTransform() {
			glm::vec3 position, scale, skew;
			glm::quat qua;
			glm::vec4 perspective;
			glm::decompose(Transform, scale, qua, position, skew, perspective);

			return { position, qua, scale };
		}
		void SetPosition(const glm::vec3& postion) {
			Transform[3][0] = postion.x;
			Transform[3][1] = postion.y;
			Transform[3][2] = postion.z;
		}
	};

///////////////////////////////////////////////////// Rendering components /////////////////////////////////////////////////////
	struct SpriteRendererComponent {
		glm::vec4 TintColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		Pixelate::Rect Rect;
		Ref<Pixelate::Texture> Texture;



		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent& other) 
			: TintColor(other.TintColor), Rect(other.Rect), Texture(other.Texture) {}

		SpriteRendererComponent(const glm::vec4& tintColor, const Pixelate::Rect& rect, const Ref<Pixelate::Texture>& texture)
			: TintColor(tintColor), Rect(rect), Texture(texture) {}


	};


	//TODO: i don't know if i agree with this design. Might refactor soon.
	struct CameraComponent {

		OrthographicCamera Camera;
		glm::vec4 ClearColor = {0.25f, 0.5f, 0.7f, 1.0f};
		bool PrimaryCamera = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent& other) : Camera(other.Camera), ClearColor(other.ClearColor), PrimaryCamera(other.PrimaryCamera) {}

	};


	struct AnimatorComponent {
		Pixelate::Animator Animator;
		AnimatorComponent() = default;
		AnimatorComponent(const AnimatorComponent& other) : Animator(other.Animator) {}
	};

// 	struct ParticleEmmiterComponent {
// 
// 	};
///////////////////////////////////////////////////// Audio components /////////////////////////////////////////////////////

	struct AudioListenerComponent {
		Pixelate::AudioListener Listener;



		AudioListenerComponent() = default;

		AudioListenerComponent(const AudioListenerComponent& comp)
			: Listener(comp.Listener)
		{
		}

		AudioListenerComponent(const AudioListener& listener)
			: Listener(listener)
		{
		}

	};

	struct AudioSourceComponent {
		Ref<AudioSource> Source;
		std::string FilePath;

		AudioSourceComponent() = default;

		AudioSourceComponent(const AudioSourceComponent& comp)
			: Source(comp.Source), FilePath(comp.FilePath)
		{ 	 
		}

		AudioSourceComponent(const Ref<AudioSource>& source, const std::string& filepath)
			: Source(source), FilePath(filepath)
		{
		}


	};

///////////////////////////////////////////////////// Physics components /////////////////////////////////////////////////////

	struct RigidBodyComponent {

		Pixelate::RigidBody RigidBody;
		Pixelate::RigidBodyDef Definition;

		RigidBodyComponent() = default;
		RigidBodyComponent(const RigidBodyComponent& other) : 
			RigidBody(other.RigidBody), Definition(other.Definition)
		{}

		
	};

	struct BoxColliderComponent {


		b2PolygonShape* ColliderData = nullptr;
		glm::vec2 Center, Size;
		bool IsTrigger = false;

		
		BoxColliderComponent() = default;
		BoxColliderComponent(glm::vec2 center, glm::vec2 size, bool isTrigger) :
			Center(center), Size(size), IsTrigger(isTrigger)
		{}

	};

	//TODO: Need to support the other colliders such as polygon, edge and circle

///////////////////////////////////////////////////// Scripting components /////////////////////////////////////////////////////

	struct ScriptingBehaviourComponent {

		ScriptBehaviour Behaviour;

		ScriptingBehaviourComponent() = default;
		ScriptingBehaviourComponent(const std::string& className) 
		{
			Behaviour.ClassName = className;
		}
		ScriptingBehaviourComponent(const ScriptingBehaviourComponent& comp)
			: Behaviour(comp.Behaviour)
		{}
	};


}