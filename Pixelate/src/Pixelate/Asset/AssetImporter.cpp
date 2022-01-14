#include "PXpch.h"
#include "AssetImporter.h"

#include "AssetManager.h"

#include "Pixelate/Rendering/API/Texture.h"
#include "Pixelate/Audio/Audio.h"
#include "Pixelate/Audio/AudioLoader.h"
#include "Pixelate/Physics/Physics.h"
#include "Pixelate/Rendering/API/Shader/Shader.h"

#include "Pixelate/Rendering/Material.h"
#include "Pixelate/Rendering/MaterialManager.h"



namespace Pixelate {



	std::unordered_map<Pixelate::AssetType, Pixelate::Ref<AssetTypeImporter>> AssetImporter::m_Importers;


	void AssetImporter::Init()
	{
		m_Importers = 
		{	{AssetType::Texture, CreateRef<TextureImporter>() },
			{AssetType::Audio, CreateRef<AudioImporter>() },
			{AssetType::PhysicsMaterial2D, CreateRef<PhysicsMaterial2DImporter>() },
			{AssetType::Shader, CreateRef<ShaderImporter>() },
			{AssetType::Material, CreateRef<MaterialImporter>() },

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
		asset = CreateRef<AudioBuffer>(AudioLoader::Load(AssetManager::GetFilePathString(metadata)));
		asset->Handle = metadata.Handle;

		return true;
	}


	void AudioImporter::Reload(Ref<Asset>& asset)
	{
		//Audio::Reload(AssetManager::GetMetadata(asset->Handle));
	}




	////////////// --------- PhysicsMaterial2DImporter --------- ////////////// 

	bool PhysicsMaterial2DImporter::TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset)
	{
		asset = PhysicsMaterial2DSerialization::Deserialize(AssetManager::GetFilePathString(metadata));
		asset->Handle = metadata.Handle;

		return true;

	}

	void PhysicsMaterial2DImporter::Reload(Ref<Asset>& asset)
	{

	}


	////////////// --------- ShaderImporter --------- ////////////// 



	bool ShaderImporter::TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset)
	{
		asset = Shader::Create(AssetManager::GetFilePathString(metadata));
		asset->Handle = metadata.Handle;

		return true;
	}

	void ShaderImporter::Reload(Ref<Asset>& asset)
	{

		Ref<Shader> shader = std::dynamic_pointer_cast<Shader>(asset);
		shader->Reload();

	}

	////////////// --------- MaterialImporter --------- ////////////// 


	bool MaterialImporter::TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset)
	{
		asset = MaterialSerialization::Deserialize(AssetManager::GetFilePathString(metadata));
		asset->Handle = metadata.Handle;
		return true;
	}

	void MaterialImporter::Reload(Ref<Asset>& asset)
	{
		//Ref<Material> mat = std::dynamic_pointer_cast<Material>(asset);
		//mat->Reload();
	}

}