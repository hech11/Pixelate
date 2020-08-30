#include "PXpch.h"
#include "ScriptingMaster.h"


#include "imgui.h"

#include "Pixelate/Utility/File.h"
#include <Pixelate\Scene\Components.h>
#include "glm/gtc/type_ptr.inl"

#include "Pixelate/Scene/Entity.h"
#include "Pixelate/Core/UUID.h"

#include "Pixelate/Scene/SceneManager.h"

namespace Pixelate {

	struct ScriptingMasterData
	{
		std::string AssemblyPath;
		MonoDomain* Domain;

		// Pixelate-scripting
		MonoAssembly* CoreAssembly;
		MonoImage* CoreAssemblyImage;


		// Client app
		MonoAssembly* AppAssembly;
		MonoImage* AppAssemblyImage;

	};

	static ScriptingMasterData s_MonoData;

	void ScriptBehaviour::InitBehaviours() {

		OnCreateFunc = CreateMethod(ClassName + ":OnCreate()");
		OnUpdateFunc = CreateMethod(ClassName + ":OnUpdate(single)");
		OnDestroyFunc = CreateMethod(ClassName + ":OnDestroy()");

	}

	void ScriptBehaviour::InitFields() {
		int numFields = mono_class_num_fields(Class);
		AllFields.resize(numFields);
		void* ptr = 0;
		MonoClassField* it;


		int i = 0;
		while ((it = mono_class_get_fields(Class, &ptr)) != 0) {
			
			AllFields[i].Field = it;
			AllFields[i].VariableName = mono_field_get_name(it);
			MonoType* type = mono_field_get_type(it);
			

			switch (mono_type_get_type(type)) {
				case MONO_TYPE_I4:
					AllFields[i].Type = PropertyType::Int;
					break;
				case MONO_TYPE_BOOLEAN:
					AllFields[i].Type = PropertyType::Bool;
					break;
				case MONO_TYPE_R4:
					AllFields[i].Type = PropertyType::Float;
					break;

			}

			mono_field_get_value(mono_gchandle_get_target(Handle), AllFields[i].Field, &AllFields[i].Value);

			
			i++;
		}

	}

	void ScriptBehaviour::SetPropertyValue(void* value, MonoClassField* field)
	{
		mono_field_set_value(mono_gchandle_get_target(Handle), field, value);
	}

	void* ScriptBehaviour::GetFieldValue(MonoClassField* field)
	{
		void* ptr = nullptr;
		mono_field_get_value(mono_gchandle_get_target(Handle), field, &ptr); // assert if cannot set the value for some reason?
		return ptr;

	}

	MonoMethod* ScriptBehaviour::CreateMethod(const std::string& methodDesc) {
		MonoMethodDesc* desc = mono_method_desc_new(methodDesc.c_str(), NULL);
		if (!desc)
			PX_CORE_ERROR("Desc failed!\n");


		MonoMethod* method = mono_method_desc_search_in_image(desc, s_MonoData.AppAssemblyImage);

		if (!method)
			PX_CORE_WARN("The method '%s' does not exist!\n", methodDesc.c_str());

		return method;
	}
	

	void ScriptingMaster::Init(const std::string& assemblyPath) {
		s_MonoData.AssemblyPath = assemblyPath;

		mono_set_assemblies_path("../Pixelate-Scripting/Pixelate-Scripting/4.5/");
		mono_jit_init("Pixelate");

		s_MonoData.Domain = mono_domain_create_appdomain((char*)"PixelateRuntime", nullptr);
		mono_domain_set(s_MonoData.Domain, false);

		// TODO: not ideal at all. fix this
		#ifdef PX_DEBUG
		std::string filepath = "../bin/Debug-windows-x86_64/Pixelate-Scripting/Pixelate-Scripting.dll";
		#endif

		#ifdef PX_RELEASE
		std::string filepath = "../bin/Release-windows-x86_64/Pixelate-Scripting/Pixelate-Scripting.dll";
		#endif

		#ifdef PX_DISTRIBUTE
		#error "Dist builds do not support scripting!"
		#endif

		s_MonoData.CoreAssembly = mono_domain_assembly_open(s_MonoData.Domain, filepath.c_str());
		if (!s_MonoData.CoreAssembly) {
			PX_CORE_ERROR("Could not open the core assembly!");

		}
		s_MonoData.CoreAssemblyImage = mono_assembly_get_image(s_MonoData.CoreAssembly);
		if(!s_MonoData.CoreAssemblyImage)
			PX_CORE_ERROR("Could not get core assembly image!");


		s_MonoData.AppAssembly = mono_domain_assembly_open(s_MonoData.Domain, assemblyPath.c_str());
		if (!s_MonoData.AppAssembly) {
			PX_CORE_ERROR("Could not open the app assembly!");

		}
		s_MonoData.AppAssemblyImage = mono_assembly_get_image(s_MonoData.AppAssembly);
		if (!s_MonoData.AppAssemblyImage)
			PX_CORE_ERROR("Could not get client assembly image!");


		RegisterAll();


	}

	void ScriptingMaster::Shutdown() {
		mono_jit_cleanup(s_MonoData.Domain);
	}



	void ScriptingMaster::ReloadAssembly() {
// 		Shutdown();
// 
// 		Init(s_MonoData.AssemblyPath);
	}

	void ScriptingMaster::OnEntityCreate(const ScriptBehaviour& sb, void** params) {

		MonoObject* exc = 0;
		if(sb.OnCreateFunc)
			mono_runtime_invoke(sb.OnCreateFunc, mono_gchandle_get_target(sb.Handle), params, &exc);
	}
	void ScriptingMaster::OnEntityUpdate(const ScriptBehaviour& sb, void** params) {

		MonoObject* exc = 0;
		if(sb.OnUpdateFunc)
			mono_runtime_invoke(sb.OnUpdateFunc, mono_gchandle_get_target(sb.Handle), params, &exc);
	}
	void ScriptingMaster::OnEntityDestroy(const ScriptBehaviour& sb, void** params) {

		MonoObject* exc = 0;
		if(sb.OnDestroyFunc)
			mono_runtime_invoke(sb.OnDestroyFunc, mono_gchandle_get_target(sb.Handle), params, &exc);
	}


	bool ScriptingMaster::ClassExists(const std::string& className)
	{
		auto c = mono_class_from_name(s_MonoData.AppAssemblyImage, "", className.c_str());
		return c != nullptr;
	}

	void ScriptingMaster::OnImguiRender()
	{

	}
	
	void ScriptingMaster::CreateEntityScript(Entity entity, ScriptBehaviour& sb) {
		

		if (sb.Class != nullptr) {
			mono_runtime_set_shutting_down();
		}



		auto uuid = entity.GetComponent<UUIDComponent>().UUID;

		sb.Class = mono_class_from_name(s_MonoData.AppAssemblyImage, "", sb.ClassName.c_str());

		MonoProperty* entityIDPropery = mono_class_get_property_from_name(sb.Class, "UUID");
		mono_property_get_get_method(entityIDPropery);
		MonoMethod* entityIDSetMethod = mono_property_get_set_method(entityIDPropery);

		if (sb.Class == nullptr) {
			PX_CORE_ERROR("Creating a class!\n");
			return;
		}

		MonoObject* obj = mono_object_new(s_MonoData.Domain, sb.Class);
		if (!obj) {
			PX_CORE_ERROR("Creating a object failed!\n");

		}
		mono_runtime_object_init(obj);
		mono_runtime_set_shutting_down();
		sb.Handle = mono_gchandle_new(obj, false);


		MonoObject* exc = 0;
		void* params = { &uuid };
		mono_runtime_invoke(entityIDSetMethod, mono_gchandle_get_target(sb.Handle), &params, &exc);

		sb.InitBehaviours();
		sb.InitFields();
	}

	void ScriptingMaster::RegisterAll() {
		// add all of Pixelate's functions and components here

		
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

	namespace Script {
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

		std::string MonoStringToStdString(MonoString* str)
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

		void Pixelate_AudioSourceComponent_Play(unsigned long long entity) {
			Ref<Scene>& scene = SceneManager::GetActiveScene();
			auto& entityMap = scene->GetEntityMap();

			auto& e = entityMap[entity].GetComponent<AudioSourceComponent>();
			e.Source->Play();

		}

		

////////////////////////////////////// Audio Source Component ///////////////////////////////////////////////////

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

	}
	

}