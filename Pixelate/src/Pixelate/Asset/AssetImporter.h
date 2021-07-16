#pragma once

#include "Asset.h"



namespace Pixelate {

	class AssetImporter {

		public :
			static bool LoadData(const AssetMetadata& metadata, Ref<Asset>& asset);

	};

}