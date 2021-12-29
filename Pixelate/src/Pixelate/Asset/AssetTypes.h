#pragma once

#include <string>
#include <unordered_map>
#include "Asset.h"

namespace Pixelate { namespace Utils {


	// are these tables needed? or should we use a switch/case inside a method instead.....

	inline static std::unordered_map<std::string, AssetType> s_AssetExtensionTypesTable = {
		{".PXScene",	AssetType::Scene },
		{".pxShader",	AssetType::Shader },
		{".ogg",		AssetType::Audio },
		{".mp3",		AssetType::Audio },
		{".wav",		AssetType::Audio },
		{".png",		AssetType::Texture },
		{".jpg",		AssetType::Texture },
		{".pxar",		AssetType::AssetRegistry },
		{".pxam",		AssetType::AudioMixer },
		{".pxPhysicsMaterial2D", AssetType::PhysicsMaterial2D }
	};

	inline static std::unordered_map<AssetType, std::string> s_AssetTypeStringTable = {
		{AssetType::Scene,				"Scene"},
		{AssetType::Shader,				"Shader"},
		{AssetType::Audio,				"Audio"},
		{AssetType::Texture,			"Texture"},
		{AssetType::AssetRegistry,		"Asset Registry"},
		{AssetType::AudioMixer,			"Audio Mixer"},
		{AssetType::PhysicsMaterial2D,	"Physics Material2D"},

	};

	inline static std::unordered_map< std::string, AssetType> s_AssetStringTypeTable = {
		{"Scene",			AssetType::Scene},
		{"Shader",			AssetType::Shader},
		{"Audio",			AssetType::Audio},
		{"Texture",			AssetType::Texture},
		{"Asset Registry",	AssetType::AssetRegistry},
		{"Audio Mixer",		AssetType::AudioMixer},
		{"Physics Material2D",		AssetType::PhysicsMaterial2D}
	};



	inline AssetType StringToAssetTypeExtension(const std::string& filepath) {
		std::filesystem::path path(filepath);
		std::string extension = path.extension().string();

		if (s_AssetExtensionTypesTable.find(extension) != s_AssetExtensionTypesTable.end()) {
			return s_AssetExtensionTypesTable[extension];
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