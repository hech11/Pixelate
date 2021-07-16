#include "PXpch.h"
#include "AssetManager.h"
#include "AssetTypes.h"

#include <imgui.h>


namespace Pixelate {





	static std::filesystem::path s_AssetPath = "assets"; //TODO: add to a "project" class

	void AssetManager::Init()
	{
		bool succsess = s_AssetRegistry.Deserialize();
		if (!succsess) {
			ReloadAssets();
			s_AssetRegistry.Serialize();
		}

	}

	void AssetManager::Shutdown()
	{
		//ReloadAssets();
		s_AssetRegistry.Serialize();
		s_AssetRegistry.GetRegistry().clear();
	}

	AssetHandle AssetManager::ImportAsset(const std::filesystem::path& filepath) {

		auto path = std::filesystem::relative(filepath, s_AssetPath); // should be the asset directory

		// TODO: If we already have this asset then theres no need to reimport it again.
		// if(has asset already...)


		AssetMetadata metadata;
		metadata.Handle = 0;

		AssetType type = Pixelate::Utils::StringToAssetTypeExtension(path.string());
		if (type != AssetType::None) {
			metadata.Handle = AssetHandle();
			metadata.Filepath = path;
			metadata.Type = type;

			s_AssetRegistry.GetRegistry()[metadata.Filepath] = metadata;
		}



		return metadata.Handle;

	}

	void AssetManager::ReloadAssets()
	{
		ProcessDirectoryWhenReloading(s_AssetPath);

	}



	void AssetManager::ProcessDirectoryWhenReloading(const std::filesystem::path& dir)
	{
		for (auto& path : std::filesystem::directory_iterator(dir)) {

			if (path.is_directory())
				ProcessDirectoryWhenReloading(path.path());
			else
				ImportAsset(path.path());

		}
	}
	void AssetManager::OnImguiRender(bool open)
	{
		if (!open)
			return;

		ImGui::Begin("Asset Manager");

		for (const auto& [path, metadata] : s_AssetRegistry.GetRegistry()) {

			ImGui::Text("Handle: %s", std::to_string(metadata.Handle).c_str());
			ImGui::Text("Filepath: %s", metadata.Filepath.string().c_str());
			ImGui::Text("Type: %s", Pixelate::Utils::AssetTypeToString(metadata.Type).c_str());
			ImGui::Separator();

		}

		ImGui::End();
	}


}