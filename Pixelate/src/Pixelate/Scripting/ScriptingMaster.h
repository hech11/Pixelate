#pragma once

#include <string>
#include <mono/metadata/assembly.h>
#include <mono/jit/jit.h>
#include <mono/metadata/debug-helpers.h>

#include <unordered_map>


#include "Pixelate/Scene/Scene.h"

#include <glm/glm.hpp>
#include "Pixelate/Core/Input.h"


namespace Pixelate {

	enum class PropertyType {
		None = -1,
		Bool,
		Int,
		Float,
		Vec2,
		Vec3,
		Vec4
	};

	struct ScriptPublicProperty {
		std::string VariableName;
		PropertyType Type;
		void* Value;

		MonoClassField* Field;
	};


	struct ScriptBehaviour {
		std::string ClassName;
		std::vector<ScriptPublicProperty> AllFields;

		MonoClass* Class = nullptr;
		unsigned int Handle;
		MonoMethod* OnCreateFunc = nullptr, *OnUpdateFunc = nullptr, 
			*OnDestroyFunc = nullptr, *OnCollisionEnterFunc = nullptr, 
			*OnCollisionExitFunc = nullptr;


		void InitBehaviours();
		void InitFields();

		void SetPropertyValue(void* value, MonoClassField* field);
		void* GetFieldValue(MonoClassField* field);

	private:
		MonoMethod* CreateMethod(const std::string& methodDesc);
	};



	class ScriptingMaster {

		public :
			static void Init(const std::string& assemblyPath);
			static void Shutdown();

			static void ReloadAssembly();

			static void OnEntityCreate(const ScriptBehaviour& sb, void** params = nullptr);
			static void OnEntityUpdate(const ScriptBehaviour& sb, void** params = nullptr);
			static void OnEntityDestroy(const ScriptBehaviour& sb, void** params = nullptr);
			static void OnEntityCollisionEnter(const ScriptBehaviour& sb, UUID params);
			static void OnEntityCollisionExit(const ScriptBehaviour& sb, void* params = nullptr);

			static bool ClassExists(const std::string& className);

			// used for testing
			static void OnImguiRender();
			static void CreateEntityScript(Entity entity, ScriptBehaviour& sb);
			
	};



}