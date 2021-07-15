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

		private :
			static void ProcessDirectoryWhenReloading(const std::filesystem::path& dir);
		private :
			inline static AssetRegistry s_AssetRegistry;

	};

}