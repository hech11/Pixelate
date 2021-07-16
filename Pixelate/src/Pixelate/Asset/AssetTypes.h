#pragma once

#include <string>
#include <unordered_map>
#include "Asset.h"

namespace Pixelate { namespace Utils {


	// are these tables needed? or should we use a switch/case inside a method instead.....

	inline static std::unordered_map<std::string, AssetType> s_AssetTypesTable = {
		{".PXScene",	AssetType::Scene },
		{".shader",		AssetType::Shader },
		{".Shader",		AssetType::Shader },
		{".ogg",		AssetType::Audio },
		{".mp3",		AssetType::Audio },
		{".wav",		AssetType::Audio },
		{".png",		AssetType::Texture },
		{".jpg",		AssetType::Texture },
		{".pxar",		AssetType::AssetRegistry }
	};

	inline static std::unordered_map<AssetType, std::string> s_AssetTypeStringTable = {
		{AssetType::Scene,			"Scene"},
		{AssetType::Shader,			"Shader"},
		{AssetType::Audio,			"Audio"},
		{AssetType::Texture,		"Texture"},
		{AssetType::AssetRegistry,	"Asset Registry"}
	};

	inline static std::unordered_map< std::string, AssetType> s_AssetStringTypeTable = {
		{"Scene",			AssetType::Scene},
		{"Shader",			AssetType::Shader},
		{"Audio",			AssetType::Audio},
		{"Texture",			AssetType::Texture},
		{"Asset Registry",	AssetType::AssetRegistry}
	};



	inline AssetType StringToAssetTypeExtension(const std::string& filepath) {
		std::filesystem::path path(filepath);
		std::string extension = path.extension().string();

		if (s_AssetTypesTable.find(extension) != s_AssetTypesTable.end()) {
			return s_AssetTypesTable[extension];
		}

		return AssetType::None;
	}

	inline AssetType StringToAssetType(const std::string& type) {
		return s_AssetStringTypeTable[type];
	}

	inline std::string AssetTypeToString(AssetType type) {
		return s_AssetTypeStringTable[type];
	}





} }