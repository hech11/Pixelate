#include "PXpch.h"
#include "ScriptingMaster.h"

#include <mono/jit/jit.h>
#include <mono/metadata/debug-helpers.h>

#include "imgui.h"

#include "Pixelate/Utility/File.h"

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

	MonoMethod* ScriptBehaviour::CreateMethod(const std::string& methodDesc) {
		MonoMethodDesc* desc = mono_method_desc_new(methodDesc.c_str(), NULL);
		if (!desc)
			PX_CORE_ERROR("Desc failed!\n");


		MonoMethod* method = mono_method_desc_search_in_image(desc, s_MonoData.AppAssemblyImage);

		if (!method)
			PX_CORE_ERROR("Method failed!\n");

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

		//s_MonoData.CoreAssembly = LoadAssemblyFromFile(filepath);
		s_MonoData.CoreAssembly = mono_domain_assembly_open(s_MonoData.Domain, filepath.c_str());
		if (!s_MonoData.CoreAssembly) {
			PX_CORE_ERROR("Could not open the core assembly!");

		}
		s_MonoData.CoreAssemblyImage = mono_assembly_get_image(s_MonoData.CoreAssembly);
		if(!s_MonoData.CoreAssemblyImage)
			PX_CORE_ERROR("Could not get core assembly image!");


		//s_MonoData.AppAssembly = LoadAssemblyFromFile(assemblyPath);
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
	
	void ScriptingMaster::CreateEntityScript(ScriptBehaviour& sb) {
		

		if (sb.Class != nullptr) {
			mono_runtime_set_shutting_down();
		}


		sb.Class = mono_class_from_name(s_MonoData.AppAssemblyImage, "", sb.ClassName.c_str());
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

		sb.InitBehaviours();
	}

	void ScriptingMaster::RegisterAll() {
		// add all of Pixelate's functions and components here

		

	}

	

	

}