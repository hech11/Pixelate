#pragma once

#include <string>
#include "AssetRegistry.h"
#include "AssetImporter.h"

namespace Pixelate {


	struct AssetManagerConfiguration {
		std::string AssetDirectory = "assets";
	};

	class AssetManager {

		public :

			static void Init();
			static void Shutdown();



			static std::filesystem::path GetFilePath(const AssetMetadata& metadata) { return s_AssetPath / metadata.Filepath; }
			static std::string GetFilePathString(const AssetMetadata& metadata) { return GetFilePath(metadata).string(); }

			static AssetMetadata& GetMetadata(AssetHandle handle);
			static bool IsAssetHandleValid(AssetHandle handle) { return GetMetadata(handle).IsValid(); }

			static AssetHandle ImportAsset(const std::filesystem::path& filepath);
			static void ReloadAssets();

			static void OnImguiRender(bool open);


			template<typename T>
			static Ref<T> GetAsset(const std::string& filepath) {
				auto& metadata = s_AssetRegistry.GetRegistry()[filepath];

				Ref<Asset> asset = nullptr;
				if (!metadata.IsLoaded)
				{
					metadata.IsLoaded = AssetImporter::LoadData(metadata, asset);
					if (!metadata.IsLoaded)
						return nullptr;

					// store the loaded data into cache

				} else {
					// load it from a cache
				}

				return Ref<T>(*asset);
			}

			template<typename T>
			static Ref<T> GetAsset(AssetHandle handle) {
				auto& metadata = GetMetadata(handle);

				Ref<Asset> asset = nullptr;
				if (!metadata.IsLoaded)
				{
					metadata.IsLoaded = AssetImporter::LoadData(metadata, asset);
					if (!metadata.IsLoaded)
						return nullptr;

					// store the loaded data into cache
					s_LoadedAssets[metadata.Handle] = asset;
				}
				else {
					// load it from a cache
					return std::dynamic_pointer_cast<T>(s_LoadedAssets[metadata.Handle]);
				}
				
				return  std::dynamic_pointer_cast<T>(asset);
			}

		private :
			static void ProcessDirectoryWhenReloading(const std::filesystem::path& dir);
		private :
			static std::filesystem::path s_AssetPath; //TODO: add to a "project" class
			static AssetMetadata s_NullMetadata;
			inline static AssetRegistry s_AssetRegistry;
			static std::unordered_map<AssetHandle, Ref<Asset>> s_LoadedAssets;

			static std::string m_FilterBuffer;
			static bool m_IsFiltering;
			

	};

}