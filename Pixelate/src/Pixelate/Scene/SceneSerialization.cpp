#include "PXpch.h"
#include "SceneSerialization.h"
#include "yaml-cpp/yaml.h"

#include "Pixelate/Scene/Components.h"
#include "Pixelate/Scene/Entity.h"


#include <fstream>

namespace Pixelate {



	void SceneSerialization::Serialize(const Ref<Scene>& scene, const std::string& filepath) {
		YAML::Emitter data;
		data << YAML::BeginMap;
		data << YAML::Key << "Scene";
		data << YAML::Value << scene->GetName();
		data << YAML::Key << "Entities";
		data << YAML::Value << YAML::BeginSeq;

		scene->GetReg().each([&](auto entity) {
			Entity e = { entity, scene.get() };

			if (!e.HasComponent<UUIDComponent>()) {
				return;
			}



			data << YAML::BeginMap;
			data << YAML::Key << "Entity";
			data << YAML::Value << e.GetComponent<UUIDComponent>().UUID;


			//TODO: Support all components
			if (e.HasComponent<NameComponent>()) {

				data << YAML::Key << "NameComponent";

				data << YAML::BeginMap;
				data << YAML::Key << "Name" << YAML::Value << e.GetComponent<NameComponent>().Name;
				data << YAML::EndMap;


			}

		});

		data << YAML::EndSeq;
		data << YAML::EndMap;


		std::ofstream file(filepath);
		file << data.c_str();
	}

// 	Ref<Scene> SceneSerialization::Deserialize(const std::string& filepath) {
// 		PX_ASSERT(false, "Not implemented");
// 	}

}