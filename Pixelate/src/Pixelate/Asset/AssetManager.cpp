#include "PXpch.h"
#include "AssetManager.h"
#include "AssetTypes.h"

#include <imgui.h>


namespace Pixelate {





	std::filesystem::path AssetManager::s_AssetPath = "assets"; //TODO: add to a "project" class
	Pixelate::AssetMetadata AssetManager::s_NullMetadata;

	std::unordered_map<AssetHandle, Ref<Asset>> AssetManager::s_LoadedAssets;


	std::string AssetManager::m_FilterBuffer;
	bool AssetManager::m_IsFiltering;


	void AssetManager::Init()
	{
		s_NullMetadata.Type = AssetType::None;

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

	AssetMetadata& AssetManager::GetMetadata(AssetHandle handle)
	{
		for (auto& [filepath, metadata] : s_AssetRegistry.GetRegistry()) {
			if (metadata.Handle == handle)
				return metadata;
		}

		return s_NullMetadata;
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

		char buffer[255];
		memset(buffer, 0, 255);
		memcpy(buffer, m_FilterBuffer.c_str(), m_FilterBuffer.length());

		if (ImGui::InputText("Filter", buffer, 255)) {
			m_FilterBuffer = buffer;
		}
		if (m_FilterBuffer != "")
			m_IsFiltering = true;
		else
			m_IsFiltering = false;

		ImGui::Separator();
		for (const auto& [path, metadata] : s_AssetRegistry.GetRegistry()) {

			if (!m_IsFiltering) {
				ImGui::Text("Handle: %s", std::to_string(metadata.Handle).c_str());
				ImGui::Text("Filepath: %s", metadata.Filepath.string().c_str());
				ImGui::Text("Type: %s", Pixelate::Utils::AssetTypeToString(metadata.Type).c_str());

				ImGui::Separator();
			} else {
				auto filename = metadata.Filepath.filename().string();
				if (filename.find(m_FilterBuffer) < filename.length()) {
					ImGui::Text("Handle: %s", std::to_string(metadata.Handle).c_str());
					ImGui::Text("Filepath: %s", metadata.Filepath.string().c_str());
					ImGui::Text("Type: %s", Pixelate::Utils::AssetTypeToString(metadata.Type).c_str());

					ImGui::Separator();
				}
			}

		}

		ImGui::End();
	}


}