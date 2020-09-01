#include "PXpch.h"
#include "ScriptingRegistery.h"

#include "Pixelate\Scene\Components.h"
#include "glm/gtc/type_ptr.inl"

#include "Pixelate/Scene/Entity.h"
#include "Pixelate/Core/UUID.h"

#include "Pixelate/Scene/SceneManager.h"

namespace Pixelate { namespace Script {



	void RegisterMethods() {


		mono_add_internal_call("Pixelate.Entity::GetTransform_CPP", Script::Pixelate_Entity_GetTransform);
		mono_add_internal_call("Pixelate.Entity::SetTransform_CPP", Script::Pixelate_Entity_SetTransform);

		mono_add_internal_call("Pixelate.SceneManager::LoadScene_CPP", Script::Pixelate_SceneManager_LoadScene);


		mono_add_internal_call("Pixelate.Input::IsKeyDown_CPP", (void*)Script::Pixelate_Input_IsKeyDown);
		mono_add_internal_call("Pixelate.Input::IsMouseButtonDown_CPP", (void*)Script::Pixelate_Input_IsMouseButtonDown);
		mono_add_internal_call("Pixelate.Input::GetMousePosition_CPP", (void*)Script::Pixelate_Input_GetMousePosition);

		mono_add_internal_call("Pixelate.RigidBodyComponent::SetLinearVelocity_CPP", Script::Pixelate_RigidbodyComponent_SetLinearVelocity);
		mono_add_internal_call("Pixelate.RigidBodyComponent::GetLinearVelocity_CPP", Script::Pixelate_RigidbodyComponent_GetLinearVelocity);
		mono_add_internal_call("Pixelate.RigidBodyComponent::SetCollisionDetection_CPP", Script::Pixelate_RigidbodyComponent_SetCollisionDetection);
		mono_add_internal_call("Pixelate.RigidBodyComponent::GetCollisionDetection_CPP", Script::Pixelate_RigidbodyComponent_GetCollisionDetection);
		mono_add_internal_call("Pixelate.RigidBodyComponent::SetSleepingState_CPP", Script::Pixelate_RigidbodyComponent_SetSleepingState);
		mono_add_internal_call("Pixelate.RigidBodyComponent::GetSleepingState_CPP", Script::Pixelate_RigidbodyComponent_GetSleepingState);
		mono_add_internal_call("Pixelate.RigidBodyComponent::SetBodyType_CPP", Script::Pixelate_RigidbodyComponent_SetBodyType);
		mono_add_internal_call("Pixelate.RigidBodyComponent::GetBodyType_CPP", Script::Pixelate_RigidbodyComponent_GetBodyType);



		mono_add_internal_call("Pixelate.CameraComponent::SetClearColor_CPP", Script::Pixelate_CamreaComponent_SetClearColor);

		mono_add_internal_call("Pixelate.SpriteRendererComponent::SetTint_CPP", Script::Pixelate_SpriteRendererComponent_SetTint);
		mono_add_internal_call("Pixelate.SpriteRendererComponent::GetTint_CPP", Script::Pixelate_SpriteRendererComponent_GetTint);

		mono_add_internal_call("Pixelate.SpriteRendererComponent::SetTextureRect_CPP", Script::Pixelate_SpriteRendererComponent_SetTextureRect);
		mono_add_internal_call("Pixelate.SpriteRendererComponent::GetTextureRect_CPP", Script::Pixelate_SpriteRendererComponent_GetTextureRect);


		mono_add_internal_call("Pixelate.AudioSourceComponent::Play_CPP", Script::Pixelate_AudioSourceComponent_Play);
		mono_add_internal_call("Pixelate.AudioSourceComponent::IsPlaying_CPP", (void*)Script::Pixelate_AudioSourceComponent_IsPlaying);
		mono_add_internal_call("Pixelate.AudioSourceComponent::Pause_CPP", Script::Pixelate_AudioSourceComponent_Pause);
		mono_add_internal_call("Pixelate.AudioSourceComponent::Stop_CPP", Script::Pixelate_AudioSourceComponent_Stop);
		mono_add_internal_call("Pixelate.AudioSourceComponent::SetLooping_CPP", Script::Pixelate_AudioSourceComponent_SetLooping);
		mono_add_internal_call("Pixelate.AudioSourceComponent::SetGain_CPP", Script::Pixelate_AudioSourceComponent_SetGain);
		mono_add_internal_call("Pixelate.AudioSourceComponent::GetPitch_CPP", (void*)Script::Pixelate_AudioSourceComponent_GetPitch);
		mono_add_internal_call("Pixelate.AudioSourceComponent::GetGain_CPP", (void*)Script::Pixelate_AudioSourceComponent_GetGain);
		mono_add_internal_call("Pixelate.AudioSourceComponent::IsLooping_CPP", (void*)Script::Pixelate_AudioSourceComponent_IsLooping);

	}


	////////////////////////////////////// Transform Component ///////////////////////////////////////////////////
	void Pixelate_Entity_SetTransform(unsigned long long entity, glm::mat4* setTransform) {
		Ref<Scene>& scene = SceneManager::GetActiveScene();
		auto& entityMap = scene->GetEntityMap();

		auto& t = entityMap[entity].GetComponent<TransformComponent>();
		memcpy(glm::value_ptr(t.Transform), setTransform, sizeof(glm::mat4));

	}
	void Pixelate_Entity_GetTransform(unsigned long long entity, glm::mat4* getTransform) {
		Ref<Scene>& scene = SceneManager::GetActiveScene();
		auto& entityMap = scene->GetEntityMap();

		auto& t = entityMap[entity].GetComponent<TransformComponent>();
		memcpy(getTransform, glm::value_ptr(t.Transform), sizeof(glm::mat4));
	}

	////////////////////////////////////// SceneManager ///////////////////////////////////////////////////

	static std::string MonoStringToStdString(MonoString* str)
	{
		auto chl = mono_string_chars(str);
		std::string result("");
		for (int i = 0; i < mono_string_length(str); i++) {
			result += chl[i];
		}
		return result;
	}
	void Pixelate_SceneManager_LoadScene(MonoString* filepath) {
		SceneManager::QueueLoadedScene(MonoStringToStdString(filepath));
	}

	////////////////////////////////////// Input ///////////////////////////////////////////////////
	bool Pixelate_Input_IsKeyDown(KeyCode* code) {
		return Input::IsKeyDown(*code);
	}

	bool Pixelate_Input_IsMouseButtonDown(MouseButton* code) {
		return Input::IsMouseButtonDown(*code);
	}



	void Pixelate_Input_GetMousePosition(glm::vec2* position) {
		auto pos = Input::GetMousePos();
		position->x = pos.first;
		position->y = pos.second;
	}

	////////////////////////////////////// Rigidbody Component ///////////////////////////////////////////////////

	void Pixelate_RigidbodyComponent_SetLinearVelocity(unsigned long long entity, glm::vec2* velocity) {
		Ref<Scene>& scene = SceneManager::GetActiveScene();
		auto& entityMap = scene->GetEntityMap();

		auto& r = entityMap[entity].GetComponent<RigidBodyComponent>();
		r.RigidBody.SetLinearVelocity(*velocity);

	}

	void Pixelate_RigidbodyComponent_GetLinearVelocity(unsigned long long entity, glm::vec2* velocity) {
		Ref<Scene>& scene = SceneManager::GetActiveScene();
		auto& entityMap = scene->GetEntityMap();

		auto& r = entityMap[entity].GetComponent<RigidBodyComponent>();
		velocity = &r.RigidBody.GetLinearVelocity();
	}
	void Pixelate_RigidbodyComponent_SetCollisionDetection(unsigned long long entity, CollisionDetectionMode* mode) {
		Ref<Scene>& scene = SceneManager::GetActiveScene();
		auto& entityMap = scene->GetEntityMap();

		auto& r = entityMap[entity].GetComponent<RigidBodyComponent>();
		r.RigidBody.SetCollisionDetectionMode(*mode);
	}
	void Pixelate_RigidbodyComponent_GetCollisionDetection(unsigned long long entity, CollisionDetectionMode* mode) {
		Ref<Scene>& scene = SceneManager::GetActiveScene();
		auto& entityMap = scene->GetEntityMap();

		auto& r = entityMap[entity].GetComponent<RigidBodyComponent>();
		*mode = r.RigidBody.GetCollisionDetectionMode();
	}
	void Pixelate_RigidbodyComponent_SetSleepingState(unsigned long long entity, SleepingState* state) {
		Ref<Scene>& scene = SceneManager::GetActiveScene();
		auto& entityMap = scene->GetEntityMap();

		auto& r = entityMap[entity].GetComponent<RigidBodyComponent>();
		r.RigidBody.SetSleepState(*state);
	}
	void Pixelate_RigidbodyComponent_GetSleepingState(unsigned long long entity, SleepingState* state) {
		Ref<Scene>& scene = SceneManager::GetActiveScene();
		auto& entityMap = scene->GetEntityMap();

		auto& r = entityMap[entity].GetComponent<RigidBodyComponent>();
		*state = r.RigidBody.GetSleepingState();
	}
	void Pixelate_RigidbodyComponent_SetBodyType(unsigned long long entity, BodyType* type) {
		Ref<Scene>& scene = SceneManager::GetActiveScene();
		auto& entityMap = scene->GetEntityMap();

		auto& r = entityMap[entity].GetComponent<RigidBodyComponent>();
		r.RigidBody.SetBodyType(*type);
	}
	void Pixelate_RigidbodyComponent_GetBodyType(unsigned long long entity, BodyType* type) {
		Ref<Scene>& scene = SceneManager::GetActiveScene();
		auto& entityMap = scene->GetEntityMap();

		auto& r = entityMap[entity].GetComponent<RigidBodyComponent>();
		*type = r.RigidBody.GetBodyType();
	}




	////////////////////////////////////// Camera Component ///////////////////////////////////////////////////

	void Pixelate_CamreaComponent_SetClearColor(unsigned long long entity, glm::vec4* clearColor) {
		Ref<Scene>& scene = SceneManager::GetActiveScene();
		auto& entityMap = scene->GetEntityMap();

		auto& c = entityMap[entity].GetComponent<CameraComponent>();
		c.ClearColor = *clearColor;

	}
	////////////////////////////////////// Sprite Renderer Component ///////////////////////////////////////////////////

	void Pixelate_SpriteRendererComponent_SetTint(unsigned long long entity, glm::vec4* tint) {
		Ref<Scene>& scene = SceneManager::GetActiveScene();
		auto& entityMap = scene->GetEntityMap();

		auto& sr = entityMap[entity].GetComponent<SpriteRendererComponent>();
		sr.TintColor = *tint;

	}

	glm::vec4* Pixelate_SpriteRendererComponent_GetTint(unsigned long long entity)
	{
		Ref<Scene>& scene = SceneManager::GetActiveScene();
		auto& entityMap = scene->GetEntityMap();

		auto& sr = entityMap[entity].GetComponent<SpriteRendererComponent>();
		return &sr.TintColor;
	}


	void Pixelate_SpriteRendererComponent_SetTextureRect(unsigned long long entity, glm::vec4* coords) {
		Ref<Scene>& scene = SceneManager::GetActiveScene();
		auto& entityMap = scene->GetEntityMap();

		auto& sr = entityMap[entity].GetComponent<SpriteRendererComponent>();
		sr.SpriteRect->SetBounds(*coords);

	}

	glm::vec4* Pixelate_SpriteRendererComponent_GetTextureRect(unsigned long long entity)
	{
		Ref<Scene>& scene = SceneManager::GetActiveScene();
		auto& entityMap = scene->GetEntityMap();

		auto& sr = entityMap[entity].GetComponent<SpriteRendererComponent>();
		glm::vec4 temp = { sr.SpriteRect->GetBounds().x, sr.SpriteRect->GetBounds().y, sr.SpriteRect->GetBounds().z, sr.SpriteRect->GetBounds().w };
		return &temp;
	}


	////////////////////////////////////// Audio Source Component ///////////////////////////////////////////////////

	void Pixelate_AudioSourceComponent_Play(unsigned long long entity) {
		Ref<Scene>& scene = SceneManager::GetActiveScene();
		auto& entityMap = scene->GetEntityMap();

		auto& e = entityMap[entity].GetComponent<AudioSourceComponent>();
		e.Source->Play();

	}

	void Pixelate_AudioSourceComponent_Pause(unsigned long long entity)
	{
		Ref<Scene>& scene = SceneManager::GetActiveScene();
		auto& entityMap = scene->GetEntityMap();

		auto& e = entityMap[entity].GetComponent<AudioSourceComponent>();
		e.Source->Pause();
	}

	void Pixelate_AudioSourceComponent_Stop(unsigned long long entity)
	{
		Ref<Scene>& scene = SceneManager::GetActiveScene();
		auto& entityMap = scene->GetEntityMap();

		auto& e = entityMap[entity].GetComponent<AudioSourceComponent>();
		e.Source->Stop();
	}

	void Pixelate_AudioSourceComponent_SetLooping(unsigned long long entity, bool loop)
	{
		Ref<Scene>& scene = SceneManager::GetActiveScene();
		auto& entityMap = scene->GetEntityMap();

		auto& e = entityMap[entity].GetComponent<AudioSourceComponent>();
		e.Source->SetLooping(loop);
	}

	void Pixelate_AudioSourceComponent_SetGain(unsigned long long entity, float gain)
	{
		Ref<Scene>& scene = SceneManager::GetActiveScene();
		auto& entityMap = scene->GetEntityMap();

		auto& e = entityMap[entity].GetComponent<AudioSourceComponent>();
		e.Source->SetGain(gain);
	}

	float Pixelate_AudioSourceComponent_GetPitch(unsigned long long entity)
	{
		Ref<Scene>& scene = SceneManager::GetActiveScene();
		auto& entityMap = scene->GetEntityMap();

		auto& e = entityMap[entity].GetComponent<AudioSourceComponent>();
		return e.Source->GetPitch();
	}

	float Pixelate_AudioSourceComponent_GetGain(unsigned long long entity)
	{
		Ref<Scene>& scene = SceneManager::GetActiveScene();
		auto& entityMap = scene->GetEntityMap();

		auto& e = entityMap[entity].GetComponent<AudioSourceComponent>();
		return e.Source->GetGain();
	}

	bool Pixelate_AudioSourceComponent_IsPlaying(unsigned long long entity)
	{
		Ref<Scene>& scene = SceneManager::GetActiveScene();
		auto& entityMap = scene->GetEntityMap();

		auto& e = entityMap[entity].GetComponent<AudioSourceComponent>();
		return e.Source->IsPlaying();
	}
	bool Pixelate_AudioSourceComponent_IsLooping(unsigned long long entity)
	{
		Ref<Scene>& scene = SceneManager::GetActiveScene();
		auto& entityMap = scene->GetEntityMap();

		auto& e = entityMap[entity].GetComponent<AudioSourceComponent>();
		return e.Source->IsLooping();
	}


} }