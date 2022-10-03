#include "PXpch.h"
#include "SortingLayers.h"

#include <yaml-cpp/yaml.h>

namespace Pixelate {

	std::vector<std::pair<RenderLayer, std::string>> SortingLayerManager::s_RenderingLayers = { {0, "Default"}, {1, "Player"} };

	uint32_t SortingLayerManager::s_LayerIndex = 2;
	uint32_t SortingLayerManager::s_LayerNumberLimit = 31;



	void SortingLayerManager::AddLayer(const std::string& name)
	{
		if (s_LayerIndex >= s_LayerNumberLimit)
			return;
		s_RenderingLayers.push_back(std::make_pair(s_LayerIndex, name));
		s_LayerIndex++;
		Serialize("resources/RenderingSettings.pxsettings");
	}

	void SortingLayerManager::RemoveLayer(int index)
	{

		s_LayerIndex--;

		int i = 0;
		for (auto& layer : s_RenderingLayers)
		{
			if (layer.first == index)
			{
				s_RenderingLayers.erase(s_RenderingLayers.begin() + i);
				break;
			}
			i++;
		}

		// sorting the layers to the indexes are still in ascending order when deleting a layer.
		std::sort(s_RenderingLayers.begin(), s_RenderingLayers.end(), [](auto& lhs, auto& rhs) {
			return lhs.first < rhs.first;
			});

		for (int i = 0; i < s_LayerIndex; i++)
		{
			s_RenderingLayers[i].first = i;
		}

		Serialize("resources/RenderingSettings.pxsettings");

	}


	
	std::string SortingLayerManager::GetLayerName(RenderLayer index)
	{
		for (auto& layer : s_RenderingLayers)
		{
			if (layer.first == index)
				return layer.second;
		}
	}




	void SortingLayerManager::Serialize(const std::string& filepath)
	{
		YAML::Emitter data;
		data << YAML::BeginMap;


		data << YAML::Key << "Layers";
		data << YAML::Value << YAML::BeginSeq;


		for (auto& layer : s_RenderingLayers)
		{

			data << YAML::BeginMap;
			data << YAML::Key << "ID" << YAML::Value << (uint32_t)layer.first;
			data << YAML::Key << "Name" << YAML::Value << layer.second;
			data << YAML::EndMap;
		}
		data << YAML::EndSeq;
		data << YAML::EndMap;


		std::ofstream file(filepath);
		file << data.c_str();

		file.close();
	}

	void SortingLayerManager::Deserialize(const std::string& filepath)
	{


		std::ifstream file(filepath);
		YAML::Node data = YAML::Load(file);
		YAML::Node layers = data["Layers"];

		if (!layers)
		{
			PX_CORE_ERROR("Could not load sorting layer file at: '%s'!\n", filepath.c_str());
			return;
		}
		else
		{
			s_RenderingLayers.clear();
			s_LayerIndex = 0;

			for (auto layer : layers)
			{
				uint32_t id = layer["ID"].as<uint32_t>();
				std::string name = layer["Name"].as<std::string>();
				s_RenderingLayers.push_back(std::make_pair(id, name ) );
				s_LayerIndex++;
			}
		}

	}


}