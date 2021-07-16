#pragma once

#include <string>
#include "AssetRegistry.h"

namespace Pixelate {


	struct AssetManagerConfiguration {
		std::string AssetDirectory = "assets";
	};

	class AssetManager {

		public :

			static void Init();
			static void Shutdown();

			static AssetHandle ImportAsset(const std::filesystem::path& filepath);
			static void ReloadAssets();

			static void OnImguiRender(bool open);



			template<typename T>
			static Ref<T> GetAsset(const std::string& filepath) {
				auto& metadata = s_AssetRegistry.GetRegistry()[filepath];

				Ref<Asset> asset = nullptr;
				if (!metadata.IsLoaded)
				{

					// TODO: loadData(metadata, asset);
					// store the loaded data into cache

				} else {
					// load it from a cache
				}


				return Ref<T>(*asset);
			}

		private :
			static void ProcessDirectoryWhenReloading(const std::filesystem::path& dir);
		private :
			inline static AssetRegistry s_AssetRegistry;

	};

}