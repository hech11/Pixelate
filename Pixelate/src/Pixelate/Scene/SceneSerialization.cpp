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

			// TODO: Clear color
			if (e.HasComponent<CameraComponent>()) {

				data << YAML::Key << "CameraComponent";

				data << YAML::BeginMap;
				data << YAML::Key << "OrthoSize" << YAML::Value << e.GetComponent<CameraComponent>().Camera.GetOrthographicSize();
				data << YAML::Key << "Primary" << YAML::Value << e.GetComponent<CameraComponent>().PrimaryCamera;
				data << YAML::EndMap;

			}

			if (e.HasComponent<AudioListenerComponent>()) {

				data << YAML::Key << "AudioListenerComponent";

				data << YAML::BeginMap;
				data << YAML::Key << "Spatial" << YAML::Value << e.GetComponent<AudioListenerComponent>().Listener.IsSpatial();
				data << YAML::EndMap;

			}
			data << YAML::EndMap;

		});

		data << YAML::EndSeq;
		data << YAML::EndMap;


		std::ofstream file(filepath);
		file << data.c_str();

		file.close();
	}

 	Ref<Scene> SceneSerialization::Deserialize(const std::string& filepath) {
		std::ifstream file(filepath);
		YAML::Node data = YAML::Load(file);

		if (!data["Scene"])
			PX_ASSERT(false, "Could not load scene!");

		std::string name = data["Scene"].as<std::string>();

		Ref<Scene> resultScene = CreateRef<Scene>(name);


		YAML::Node entities = data["Entities"];
		if (entities) {
			for (auto entity : entities) {
				unsigned long long uuid = entity["Entity"].as<unsigned long long>();
				std::string name;
				if (auto nameComp = entity["NameComponent"]) {
					name = nameComp["Name"].as<std::string>();
				}



				Entity e = resultScene->CreateEntityWithUUID(uuid, name);

				//TODO: Support all components

				if (auto cameraComp = entity["CameraComponent"]) {
					auto& comp = e.AddComponent<CameraComponent>();
					comp.Camera = OrthographicCamera();
					comp.Camera.SetOrthographicSize(cameraComp["OrthoSize"].as<float>());

					comp.PrimaryCamera = cameraComp["Primary"].as<bool>();

				}


				if (auto audioListenerComp = entity["AudioListenerComponent"]) {
					auto& comp = e.AddComponent<AudioListenerComponent>();
					comp.Listener = AudioListener();
					comp.Listener.SetSpatial(audioListenerComp["Spatial"].as<bool>());
				}


			}
		}

		file.close();
		return resultScene;
 	}

}