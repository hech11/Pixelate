#pragma once

#include <string>
#include <mono/metadata/assembly.h>
#include <unordered_map>


namespace Pixelate {


	struct ScriptBehaviour {
		std::string ClassName;

		MonoClass* Class = nullptr;
		unsigned int Handle;
		MonoMethod* OnCreateFunc = nullptr, * OnUpdateFunc = nullptr, * OnDestroyFunc = nullptr;


		void InitBehaviours();

	private:
		MonoMethod* CreateMethod(const std::string& methodDesc);
	};

	class ScriptingMaster {

		public :
			static void Init(const std::string& assemblyPath);
			static void Shutdown();

			static void OnEntityCreate(const ScriptBehaviour& sb, void** params = nullptr);
			static void OnEntityUpdate(const ScriptBehaviour& sb, void** params = nullptr);
			static void OnEntityDestroy(const ScriptBehaviour& sb, void** params = nullptr);

			static bool ClassExists(const std::string& className);

			// used for testing
			static void OnImguiRender();
			static void CreateEntityScript(ScriptBehaviour& sb);
			
		private :

			// should register all of Pixelate's components and data types but for now this is used for testing
			static void RegisterAll(); 
	


	};
}