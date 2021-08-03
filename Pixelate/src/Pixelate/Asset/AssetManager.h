#pragma once

#include <string>
#include "AssetRegistry.h"
#include "AssetImporter.h"

#include "Pixelate/Utility/FileSystem.h"


namespace Pixelate {


	struct AssetManagerConfiguration {
		std::string AssetDirectory = "assets";
	};

	class AssetManager {

		public :

			static void Init();
			static void Shutdown();

			static void SetAssetChangeCallback(const FileWatcherCallback& callback) { s_Callback = callback; }

			static void OnFileWatcherAction(FileWatcherCallbackData data);

			static std::filesystem::path GetFilePath(const AssetMetadata& metadata) { return s_AssetPath / metadata.Filepath; }
			static std::string GetFilePathString(const AssetMetadata& metadata) { return GetFilePath(metadata).string(); }

			static AssetMetadata& GetMetadata(AssetHandle handle);
			static AssetMetadata& GetMetadata(const std::filesystem::path& path);

			static bool IsAssetHandleValid(AssetHandle handle) { return GetMetadata(handle).IsValid(); }

			static AssetHandle ImportAsset(const std::filesystem::path& filepath);
			static void ReloadAssets();

			static void OnImguiRender(bool open);

			static AssetRegistry& Registry() { return s_AssetRegistry; }

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
					s_LoadedAssets[metadata.Handle] = asset;

				} else {
					// load it from a cache
					return std::dynamic_pointer_cast<T>(s_LoadedAssets[metadata.Handle]);
				}

				return  std::dynamic_pointer_cast<T>(asset);
			}

			template<typename T>
			static Ref<T> GetAsset(AssetHandle handle) {
				auto& metadata = GetMetadata(handle);

				Ref<Asset> asset = nullptr;
				if (!metadata.IsLoaded)
				{
					metadata.IsLoaded = AssetImporter::TryLoadData(metadata, asset);
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

			static void ApplyAssetChanges();

		private :
			static void ProcessDirectoryWhenReloading(const std::filesystem::path& dir);
			static void ReloadAsset(const std::filesystem::path& filepath);
			static void RemoveAssetFromRegistry(const std::filesystem::path& filepath);
			static void OnAssetRenamed(const std::filesystem::path& oldPath, const std::filesystem::path& newPath);

		private :
			static std::filesystem::path s_AssetPath; //TODO: add to a "project" class
			static AssetMetadata s_NullMetadata;
			inline static AssetRegistry s_AssetRegistry;
			static std::unordered_map<AssetHandle, Ref<Asset>> s_LoadedAssets;

			static std::vector<Ref<Asset>> s_AssetsToBeReloaded;
			static bool s_ShouldReloadAssetList;

			static std::string m_FilterBuffer;
			static bool m_IsFiltering;
			

			static FileWatcherCallback s_Callback;

	};

}