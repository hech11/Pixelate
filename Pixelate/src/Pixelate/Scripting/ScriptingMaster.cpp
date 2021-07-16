#include "PXpch.h"
#include "ScriptingMaster.h"


#include "imgui.h"

#include "Pixelate/Utility/FileSystem.h"
#include <Pixelate\Scene\Components.h>
#include "glm/gtc/type_ptr.inl"

#include "Pixelate/Scene/Entity.h"

#include "Pixelate/Scene/SceneManager.h"
#include "Pixelate/Scripting/ScriptingRegistery.h"


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
		OnDestroyFunc = CreateMethod(ClassName + ":OnDestroy()");

		OnUpdateFunc = CreateMethod(ClassName + ":OnUpdate(single)");
		OnCollisionEnterFunc = CreateMethod(ClassName + ":OnCollisionEnter(Entity)");
		OnCollisionExitFunc = CreateMethod(ClassName + ":OnCollisionExit(Entity)");

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
	

	static MonoString* StdStringToMonoString(const std::string& str)
	{
		return mono_string_new(s_MonoData.Domain, str.c_str());

	}

	void ScriptingMaster::Init(const std::string& assemblyPath) {
		s_MonoData.AssemblyPath = assemblyPath;

		mono_set_assemblies_path("../Pixelate-Scripting/Pixelate-Scripting/4.5/");
		mono_jit_init("Pixelate");

		s_MonoData.Domain = mono_domain_create_appdomain((char*)"PixelateRuntime", nullptr);
		mono_domain_set(s_MonoData.Domain, false);

		std::string filepath = "assets/scripts/Pixelate-Scripting/Pixelate-Scripting.dll";

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


		Script::RegisterMethods();


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


	void ScriptingMaster::OnEntityCollisionEnter(const ScriptBehaviour& sb, UUID params) {



		if (sb.OnCollisionEnterFunc) {

			auto c = mono_class_from_name(s_MonoData.CoreAssemblyImage, "Pixelate", "Entity");

			MonoProperty* entityIDPropery = mono_class_get_property_from_name(c, "UUID");
			mono_property_get_get_method(entityIDPropery);
			MonoMethod* entityIDSetMethod = mono_property_get_set_method(entityIDPropery);

		
			MonoProperty* entityTagPropery = mono_class_get_property_from_name(c, "Tag");
			mono_property_get_get_method(entityIDPropery);
			MonoMethod* entityTagSetMethod = mono_property_get_set_method(entityTagPropery);




			MonoObject* obj = mono_object_new(s_MonoData.Domain, c);
			mono_runtime_object_init(obj);
			mono_runtime_set_shutting_down();
			auto tag = StdStringToMonoString(SceneManager::GetActiveScene()->GetEntityMap()[params].GetComponent<TagComponent>().Tag);

			MonoObject* exc = 0;
			void* p = { &params };
			void* t = tag;
			unsigned int tempHandle = mono_gchandle_new(obj, false);
			mono_runtime_invoke(entityIDSetMethod, obj, &p, &exc);
			mono_runtime_invoke(entityTagSetMethod, obj, &t, &exc);

			void* Parameters = obj;
			

			mono_runtime_invoke(sb.OnCollisionEnterFunc, mono_gchandle_get_target(sb.Handle), &Parameters, &exc);

			mono_gchandle_free(tempHandle);
		}

	}

	void ScriptingMaster::OnEntityCollisionExit(const ScriptBehaviour& sb, void* params) {

		if (sb.OnCollisionExitFunc) {

			auto c = mono_class_from_name(s_MonoData.CoreAssemblyImage, "Pixelate", "Entity");

			MonoProperty* entityIDPropery = mono_class_get_property_from_name(c, "UUID");
			mono_property_get_get_method(entityIDPropery);
			MonoMethod* entityIDSetMethod = mono_property_get_set_method(entityIDPropery);



			MonoObject* obj = mono_object_new(s_MonoData.Domain, c);
			mono_runtime_object_init(obj);
			mono_runtime_set_shutting_down();

			MonoObject* exc = 0;
			void* p = { &params };
			unsigned int tempHandle = mono_gchandle_new(obj, false);
			mono_runtime_invoke(entityIDSetMethod, obj, &p, &exc);

			void* parameters = obj;

			mono_runtime_invoke(sb.OnCollisionExitFunc, mono_gchandle_get_target(sb.Handle), &parameters, &exc);

			mono_gchandle_free(tempHandle);
		}
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
		auto tag = StdStringToMonoString(entity.GetComponent<TagComponent>().Tag);

		sb.Class = mono_class_from_name(s_MonoData.AppAssemblyImage, "", sb.ClassName.c_str());

		MonoProperty* entityIDPropery = mono_class_get_property_from_name(sb.Class, "UUID");
		MonoProperty* entityTagPropery = mono_class_get_property_from_name(sb.Class, "Tag");
		mono_property_get_get_method(entityIDPropery);
		mono_property_get_get_method(entityTagPropery);
		MonoMethod* entityIDSetMethod = mono_property_get_set_method(entityIDPropery);
		MonoMethod* entityTagSetMethod = mono_property_get_set_method(entityTagPropery);

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
		void* paramsid = { &uuid };
		void* paramstag =  tag;
		mono_runtime_invoke(entityIDSetMethod, mono_gchandle_get_target(sb.Handle), &paramsid, &exc);
		mono_runtime_invoke(entityTagSetMethod, mono_gchandle_get_target(sb.Handle), &paramstag, &exc);

		sb.InitBehaviours();
		sb.InitFields();
	}

	
	

}