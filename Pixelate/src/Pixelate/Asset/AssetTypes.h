#pragma once

#include <string>
#include <unordered_map>
#include "Asset.h"

namespace Pixelate { namespace Utils {



	inline static std::unordered_map<std::string, AssetType> s_AssetTypesTable = {
		{".PXScene", AssetType::Scene },
		{".shader", AssetType::Shader },
		{".Shader", AssetType::Shader },
		{".ogg", AssetType::Audio },
		{".mp3", AssetType::Audio },
		{".wav", AssetType::Audio },
		{".png", AssetType::Texture },
		{".jpg", AssetType::Texture }
	};
	
	inline AssetType ConvertStringToAssetType(const std::string& filepath) {
		std::filesystem::path path(filepath);
		std::string extension = path.extension().string();

		if (s_AssetTypesTable.find(extension) != s_AssetTypesTable.end()) {
			return s_AssetTypesTable[extension];
		}

		return AssetType::None;
	}

	inline std::string ConvertAssetTypeToString(AssetType type) {
		switch (type)
		{
			case AssetType::None: return "None (possible error)";
			case AssetType::Texture: return "Texture";
			case AssetType::Audio: return "Audio";
			case AssetType::Scene: return "Scene";
			case AssetType::Shader: return "Shader";
		}

		return "No ID was listed!";

	}





} }