#pragma once

#include "Pixelate/Asset/AssetRegistry.h"

namespace Pixelate {

	class Embedded {
		public :
			static void GenerateToDisk(AssetType type, std::filesystem::path& path);

	};

	class EmbeddedGenerator {

		public :
			virtual void GenerateToDisk(std::filesystem::path path) = 0;

	};

}