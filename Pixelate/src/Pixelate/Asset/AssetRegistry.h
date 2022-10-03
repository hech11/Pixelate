#pragma once

#include "Asset.h"

#include <unordered_map>
#include <filesystem>



namespace Pixelate {

	class AssetRegistry {
		public :

			void Serialize(const std::filesystem::path& path);
			bool Deserialize(const std::filesystem::path& path, bool isResource = false);


			inline std::unordered_map<std::filesystem::path, AssetMetadata>& const GetRegistry() { return m_Registry; }

		private :
			std::unordered_map<std::filesystem::path, AssetMetadata> m_Registry;
	};

}