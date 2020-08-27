#pragma once

#include "Pixelate/Scene/Scene.h"
#include <string>



namespace Pixelate {


	class SceneSerialization {

		public :
			static void Serialize(const Ref<Scene>& scene, const std::string& filepath);
			//static Ref<Scene> Deserialize(const std::string& filepath);
	};


}