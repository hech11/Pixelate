#pragma once

#include <string>
#include <mono/metadata/assembly.h>


namespace Pixelate {

	class ScriptingMaster {

		public :
			static void Init(const std::string& assemblyPath);
			static void Shutdown();

			// used for testing
			static void OnUpdate(); 
			static void OnImguiRender();

		private :
			// should register all of Pixelate's components and data types but for now this is used for testing
			static void RegisterAll(); 
	
	};
}