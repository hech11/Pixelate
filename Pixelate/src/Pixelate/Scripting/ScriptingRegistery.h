#pragma once

#include "Pixelate/Core/KeyCodes.h"
#include "Pixelate/Core/MouseButtonCodes.h"
#include "Pixelate/Physics/RigidBody.h"
#include "glm/glm.hpp"
#include "mono/jit/jit.h"


namespace Pixelate { namespace Script {

	void RegisterMethods();


	void Pixelate_Entity_SetTransform(unsigned long long entity, glm::mat4* setTransform);
	void Pixelate_Entity_GetTransform(unsigned long long entity, glm::mat4* getTransform);

	// SceneManager
	void Pixelate_SceneManager_LoadScene(MonoString* filepath);

	// Input
	bool Pixelate_Input_IsKeyDown(KeyCode* code);
	bool Pixelate_Input_IsMouseButtonDown(MouseButton* code);
	void Pixelate_Input_GetMousePosition(glm::vec2* position);

	// Rigidbody
	void Pixelate_RigidbodyComponent_SetLinearVelocity(unsigned long long entity, glm::vec2* velocity);
	void Pixelate_RigidbodyComponent_GetLinearVelocity(unsigned long long entity, glm::vec2* velocity);
	void Pixelate_RigidbodyComponent_SetCollisionDetection(unsigned long long entity, CollisionDetectionMode* mode);
	void Pixelate_RigidbodyComponent_GetCollisionDetection(unsigned long long entity, CollisionDetectionMode* mode);
	void Pixelate_RigidbodyComponent_SetSleepingState(unsigned long long entity, SleepingState* state);
	void Pixelate_RigidbodyComponent_GetSleepingState(unsigned long long entity, SleepingState* state);
	void Pixelate_RigidbodyComponent_SetBodyType(unsigned long long entity, BodyType* type);
	void Pixelate_RigidbodyComponent_GetBodyType(unsigned long long entity, BodyType* type);


	// Camera
	void Pixelate_CamreaComponent_SetClearColor(unsigned long long entity, glm::vec4* clearColor);

	// Sprite renderer comp
	void Pixelate_SpriteRendererComponent_SetTint(unsigned long long entity, glm::vec4* tint);
	glm::vec4* Pixelate_SpriteRendererComponent_GetTint(unsigned long long entity);

	void Pixelate_SpriteRendererComponent_SetTextureRect(unsigned long long entity, glm::vec4* coord);
	glm::vec4* Pixelate_SpriteRendererComponent_GetTextureRect(unsigned long long entity);



	// Audio source
	void Pixelate_AudioSourceComponent_Play(unsigned long long entity);
	void Pixelate_AudioSourceComponent_Pause(unsigned long long entity);
	void Pixelate_AudioSourceComponent_Stop(unsigned long long entity);

	void Pixelate_AudioSourceComponent_SetLooping(unsigned long long entity, bool loop);
	void Pixelate_AudioSourceComponent_SetGain(unsigned long long entity, float gain);

	float Pixelate_AudioSourceComponent_GetPitch(unsigned long long entity);
	float Pixelate_AudioSourceComponent_GetGain(unsigned long long entity);

	bool Pixelate_AudioSourceComponent_IsPlaying(unsigned long long entity);
	bool Pixelate_AudioSourceComponent_IsLooping(unsigned long long entity);


} }