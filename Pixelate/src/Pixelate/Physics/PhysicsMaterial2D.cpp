#include "PXpch.h"
#include "PhysicsMaterial2D.h"

#include "Pixelate/Utility/FileSystem.h"

#include <yaml-cpp/yaml.h>



namespace Pixelate {




	void PhysicsMaterial2DSerialization::Serialize(const std::filesystem::path& path, const Ref<PhysicsMaterial2D>& material)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "PhysicsMaterial2D" << YAML::BeginSeq;

		out << YAML::BeginMap;
		out << YAML::Key << "Mass" << YAML::Value << material->Mass;
		out << YAML::Key << "Friction" << YAML::Value << material->Friction;
		out << YAML::Key << "Bounciness" << YAML::Value << material->Bounciness;
		out << YAML::EndMap;

		out << YAML::EndSeq;
		out << YAML::EndMap;


		std::ofstream file(path);
		file << out.c_str();

		file.close();
	}

	Ref<PhysicsMaterial2D> PhysicsMaterial2DSerialization::Deserialize(const std::filesystem::path& path)
	{
		Ref<PhysicsMaterial2D> result = nullptr;
		if (!FileSystem::Exists(path)) {
			return result;
		}

		std::ifstream stream(path);

		YAML::Node data = YAML::Load(stream);
		YAML::Node node = data["PhysicsMaterial2D"];
		if (!node) {
			PX_CORE_ERROR("Could not deduce the header of the physics material!\n");
			return result;
		}

		for (auto n : node) {
			result = CreateRef<PhysicsMaterial2D>();
			result->Mass = n["Mass"].as<float>();
			result->Friction = n["Friction"].as<float>();
			result->Bounciness = n["Bounciness"].as<float>();
		}

		return result;


	}

}