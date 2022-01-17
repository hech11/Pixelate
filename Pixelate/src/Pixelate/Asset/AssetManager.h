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

			static void AddAssetCallback(const FileWatcherCallback& callback) { s_Callbacks.push_back(callback); }

			static void OnFileWatcherAction(FileWatcherCallbackData data);

			static std::filesystem::path GetFilePath(const AssetMetadata& metadata, bool isResource = false) {
				if (isResource)
					return s_ResourcePath / metadata.Filepath;

				return s_AssetPath / metadata.Filepath;
			}
			static std::string GetFilePathString(const AssetMetadata& metadata) { return GetFilePath(metadata).string(); }

			static AssetMetadata& GetMetadata(AssetHandle handle);
			static AssetMetadata& GetMetadata(const std::filesystem::path& path);

			static bool IsAssetHandleValid(AssetHandle handle) { return GetMetadata(handle).IsValid(); }

			static AssetHandle ImportAsset(const std::filesystem::path& filepath);
			static AssetHandle ImportResource(const std::filesystem::path& filepath);

			static void ReloadAssets();
			static void ReloadResources();

			static void OnImguiRender(bool open);

			static AssetRegistry& Registry() { return s_AssetRegistry; }

			template<typename T>
			static Ref<T> GetAsset(const std::string& filepath) {
				auto path = std::filesystem::relative(filepath, s_AssetPath);

				if (path.empty())
					path = filepath;
				
				auto& metadata = s_AssetRegistry.GetRegistry()[path];


				Ref<Asset> asset = nullptr;
				if (!metadata.IsLoaded)
				{
					metadata.IsLoaded = AssetImporter::TryLoadData(metadata, asset);
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
			static void ProcessDirectoryWhenReloading(const std::filesystem::path& dir, bool isResource = false);
			static void ReloadAsset(const std::filesystem::path& filepath);
			static void RemoveAssetFromRegistry(const std::filesystem::path& filepath);
			static void RemoveResourceFromRegistry(const std::filesystem::path& filepath);
			static void OnAssetRenamed(const std::filesystem::path& oldPath, const std::filesystem::path& newPath);

		private :
			static std::filesystem::path s_AssetPath; //TODO: add to a "project" class
			static std::filesystem::path s_ResourcePath;

			static std::filesystem::path s_AssetRegistryPath;
			static std::filesystem::path s_ResourceRegistryPath;

			static AssetMetadata s_NullMetadata;
			inline static AssetRegistry s_AssetRegistry;
			inline static AssetRegistry s_ResourceRegistry;

			static std::unordered_map<AssetHandle, Ref<Asset>> s_LoadedAssets;

			static std::vector<Ref<Asset>> s_AssetsToBeReloaded;
			static bool s_ShouldReloadAssetList;

			static std::string m_FilterBuffer;
			static bool m_IsFiltering;
			

			static std::vector<FileWatcherCallback> s_Callbacks;

	};

}