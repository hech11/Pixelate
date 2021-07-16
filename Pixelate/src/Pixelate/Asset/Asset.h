#pragma once

#include "Pixelate/Core/UUID.h"
#include <filesystem>



namespace std {

	// Used for std::unordered_map
	template <>
	struct hash<std::filesystem::path> {

		std::size_t operator()(const std::filesystem::path& path) const {
			return hash_value(path);
		}
	};

}

namespace Pixelate {

	using AssetHandle = UUID;



	enum class AssetType {
		None = -1,
		Texture,
		Audio,
		Scene,
		Shader,
	};


	enum class AssetFlag {
		None = 0,
		Invalid = BIT(1),
		Missing = BIT(2)
	};

	struct AssetMetadata {
		AssetHandle Handle;
		AssetFlag Flags = AssetFlag::None;
		AssetType Type = AssetType::None;

		std::filesystem::path Filepath;
		bool IsLoaded = false;

		bool IsValid() const { return Handle != 0; }
	};

	class Asset {
		public :
			AssetHandle Handle;
			AssetFlag Flags = AssetFlag::None;

			virtual AssetType GetType() const { return AssetType::None; }

	};

}