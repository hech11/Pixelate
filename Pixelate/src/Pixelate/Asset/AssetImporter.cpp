#include "PXpch.h"
#include "AssetImporter.h"

#include "AssetManager.h"

#include "Pixelate/Rendering/API/Texture.h"
#include "Pixelate/Audio/Audio.h"



namespace Pixelate {

	bool AssetImporter::LoadData(const AssetMetadata& metadata, Ref<Asset>& asset) {


		//TODO: This is just a test. Should prob implement a interface and a list of asset importers specific to each asset
		if (metadata.Type == AssetType::Texture) {

			asset = Texture::Create(AssetManager::GetFilePathString(metadata));
			asset->Handle = metadata.Handle;

			return true;
		}

		if (metadata.Type == AssetType::Audio) {
			asset = Audio::CreateAudioSource(AssetManager::GetFilePathString(metadata), false);
			asset->Handle = metadata.Handle;

			return true;

		}


		return false;
	}

	
}