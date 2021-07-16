#include "PXpch.h"
#include "AssetRegistry.h"


#include <yaml-cpp/yaml.h>
#include "AssetTypes.h"

#include <fstream>

#include "Pixelate/Utility/FileSystem.h"

namespace Pixelate {


	static std::string s_AssetRegistryPath = "assets/AssetRegistry.pxar";

	void AssetRegistry::Serialize()
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Assets" << YAML::BeginSeq;

		for (auto& [filepath, metadata] : m_Registry) {

			out << YAML::BeginMap;
			out << YAML::Key << "Handle" << YAML::Value << metadata.Handle;
			out << YAML::Key << "Filepath" << YAML::Value << metadata.Filepath.string();
			out << YAML::Key << "Type" << YAML::Value << Utils::AssetTypeToString(metadata.Type);
			out << YAML::EndMap;
		}

		out << YAML::EndSeq;
		out << YAML::EndMap;




		std::ofstream file(s_AssetRegistryPath);
		file << out.c_str();

		file.close();

	}

	bool AssetRegistry::Deserialize()
	{
		if (!FileSystem::Exists(s_AssetRegistryPath)) {
			return false;
		}
		std::ifstream stream(s_AssetRegistryPath);
		
		YAML::Node data = YAML::Load(stream);
		auto handles = data["Assets"];
		if (!handles) {
			PX_CORE_ERROR("Could not find a asset handle. The AssetRegistry seems to be invalid!\n");
			return false;
		}


		for (auto entry : handles) {

			AssetMetadata metadata;
			metadata.Handle = entry["Handle"].as<uint64_t>();
			metadata.Filepath = entry["Filepath"].as<std::string>();
			metadata.Type = Pixelate::Utils::StringToAssetType(entry["Type"].as<std::string>());

			// validation here


			m_Registry[metadata.Filepath] = metadata;
		}


		return true;
	}

}