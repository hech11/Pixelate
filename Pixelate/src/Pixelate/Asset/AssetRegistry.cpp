#include "PXpch.h"
#include "AssetRegistry.h"


#include <yaml-cpp/yaml.h>
#include "AssetTypes.h"

#include <fstream>

#include "Pixelate/Utility/FileSystem.h"
#include "AssetManager.h"
#include "Pixelate/Debug/Instrumentor.h"

namespace Pixelate {


	static std::string s_AssetRegistryPath = "assets/AssetRegistry.pxar";

	void AssetRegistry::Serialize()
	{
		PX_PROFILE_FUNCTION();
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
		PX_PROFILE_FUNCTION();
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
			PX_PROFILE_SCOPE("AssetRegristy::SortingThroughHandles");

			AssetMetadata metadata;
			metadata.Handle = entry["Handle"].as<uint64_t>();
			std::string filepath = entry["Filepath"].as<std::string>();
			metadata.Filepath = filepath;
			metadata.Type = Pixelate::Utils::StringToAssetType(entry["Type"].as<std::string>());


			// could not find the path despite it being in our asset registry so we must search every directory to try and correct this.
			if (!FileSystem::Exists(AssetManager::GetFilePath(metadata))) {
				PX_PROFILE_SCOPE("AssetRegristy::Deserialize::FileSystem::Exists()");

				PX_CORE_WARN("Asset %s not found in registry file, trying to locate a possible match...\n", metadata.Filepath.string().c_str());


				uint32_t idealID = 0;
				std::string possibleMatch;
				for (auto& entry : std::filesystem::recursive_directory_iterator("assets/")) { // TODO: should really implement this globally soon
					
					const auto& path = entry.path();
					if(path.filename() != metadata.Filepath.filename())
						continue;



					std::string str = "/\\";

					size_t start = 0;
					size_t end = path.string().find_first_of(str);

					std::vector<std::string> result;

					while (end <= std::string::npos)
					{
						std::string token = path.string().substr(start, end - start);
						if (!token.empty())
							result.push_back(token);

						if (end == std::string::npos)
							break;

						start = end + 1;
						end = path.string().find_first_of(str, start);
					}




					uint32_t id = 0;
					for (const auto& part : result) {
						if (filepath.find(part) != std::string::npos) {
							id++;
						}

					}


					if (idealID > 0 && id == idealID)
						PX_ASSERT(false, "Not implemented");

					if(id <= idealID)
						continue;
					idealID = id;
					possibleMatch = path.string();
				}

				if (possibleMatch.empty() && idealID == 0)
				{
					PX_CORE_ERROR("Failed to locate a possible match for '%s'!\n", metadata.Filepath.string().c_str());
					continue;
				}
				std::replace(possibleMatch.begin(), possibleMatch.end(), '\\', '/');
				metadata.Filepath = std::filesystem::relative(possibleMatch, "assets");
				PX_CORE_WARN("Found a possible match: '%s'\n", metadata.Filepath.string().c_str());


			}


			m_Registry[metadata.Filepath] = metadata;
		}


		return true;
	}

}