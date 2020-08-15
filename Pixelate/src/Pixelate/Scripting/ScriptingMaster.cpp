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


		// test
		MonoObject* m_TestObject;
		MonoClass* m_TestClass;
		MonoMethod* m_TestMethod;
		unsigned int m_ObjectHandle;

	};

	static ScriptingMasterData s_MonoData;


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


	void ScriptingMaster::OnUpdate() {
		MonoObject* exc = 0;
		mono_runtime_invoke(s_MonoData.m_TestMethod, s_MonoData.m_TestObject, nullptr, &exc);
	}

	void ScriptingMaster::OnImguiRender()
	{

	}
	
	void ScriptingMaster::RegisterAll() {
		// add all of Pixelate's functions and components here

		s_MonoData.m_TestClass = mono_class_from_name(s_MonoData.CoreAssemblyImage, "", "Player");
		if (!s_MonoData.m_TestClass) {
			PX_CORE_ERROR("Creating a class!\n");
		}

		s_MonoData.m_TestObject = mono_object_new(s_MonoData.Domain, s_MonoData.m_TestClass);

		if (!s_MonoData.m_TestObject) {
			PX_CORE_ERROR("Creating a object failed!\n");

		}
		mono_runtime_object_init(s_MonoData.m_TestObject);
		s_MonoData.m_ObjectHandle = mono_gchandle_new(s_MonoData.m_TestObject, false);

		MonoMethodDesc* desc = mono_method_desc_new("Player:Print()", NULL);
		if (!desc)
			PX_CORE_ERROR("Desc failed!\n");


		s_MonoData.m_TestMethod = mono_method_desc_search_in_image(desc, s_MonoData.CoreAssemblyImage);




		if (!s_MonoData.m_TestMethod)
			PX_CORE_ERROR("Method failed!\n");

	}

	

}