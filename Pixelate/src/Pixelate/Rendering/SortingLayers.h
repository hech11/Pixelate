#pragma once

#include <string>
#include <vector>
#include <utility>

namespace Pixelate
{


	

	using RenderLayer = uint32_t;
	class SortingLayerManager
	{
		public:
			static void AddLayer(const std::string& name);

			static void RemoveLayer(int index);


			static const std::vector<std::pair<RenderLayer, std::string>>& GetLayers() { return s_RenderingLayers; }
			static const uint32_t GetMaxNumberOfLayers() { return s_LayerNumberLimit; }
			static std::string GetLayerName(RenderLayer index);
			

			static void Serialize(const std::string& filepath);
			static void Deserialize(const std::string& filepath);
		private:
			static std::vector<std::pair<RenderLayer, std::string>> s_RenderingLayers;

			static uint32_t s_LayerIndex;
			static uint32_t s_LayerNumberLimit;
	};

}