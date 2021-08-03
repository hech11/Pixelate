#include "PXpch.h"
#include "AssetImporter.h"

#include "AssetManager.h"

#include "Pixelate/Rendering/API/Texture.h"
#include "Pixelate/Audio/Audio.h"



namespace Pixelate {



	std::unordered_map<Pixelate::AssetType, Pixelate::Ref<AssetTypeImporter>> AssetImporter::m_Importers;


	void AssetImporter::Init()
	{
		m_Importers = 
		{	{AssetType::Texture, CreateRef<TextureImporter>() },
			{AssetType::Audio, CreateRef<AudioImporter>() }
		};
	}

	void AssetImporter::Shutdown()
	{
		m_Importers.clear();
	}

	bool AssetImporter::TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset) {

		AssetType type = metadata.Type;

		// debug only?
		if (m_Importers.find(type) == m_Importers.end()) {
			PX_CORE_ERROR("Could map the asset type to the appropriate asset importer!\n");
			return false;
		}

		bool success = m_Importers[type]->TryLoadData(metadata, asset);
		return success;

	}


	void AssetImporter::Reload(Ref<Asset>& asset)
	{
		AssetType type = asset->GetType();
		if (m_Importers.find(type) == m_Importers.end()) {
			PX_CORE_ERROR("Could map the asset type to the appropriate asset importer!\n");
			return;
		}

		m_Importers[type]->Reload(asset);


	}



	////////////// --------- TextureImporter --------- ////////////// 

	bool TextureImporter::TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset)
	{
		asset = Texture::Create(AssetManager::GetFilePathString(metadata));
		asset->Handle = metadata.Handle;
		return true;
	}


	void TextureImporter::Reload(Ref<Asset>& asset)
	{
		Ref<Texture> texture = std::dynamic_pointer_cast<Texture>(asset);
		texture->Reload();
	}

	////////////// --------- AudioImporter --------- ////////////// 
	bool AudioImporter::TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset)
	{
		asset = Audio::CreateAudioSource(AssetManager::GetFilePathString(metadata), false);
		asset->Handle = metadata.Handle;

		return true;
	}


	void AudioImporter::Reload(Ref<Asset>& asset)
	{
		Audio::Reload(AssetManager::GetMetadata(asset->Handle));
	}

}