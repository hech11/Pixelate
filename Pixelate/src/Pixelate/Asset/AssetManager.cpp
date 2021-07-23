#include "PXpch.h"
#include "AssetManager.h"
#include "AssetTypes.h"

#include <imgui.h>
#include "Pixelate/Debug/Instrumentor.h"
#include "Pixelate/Rendering/API/Texture.h"


namespace Pixelate {


	std::filesystem::path AssetManager::s_AssetPath = "assets"; //TODO: add to a "project" class
	Pixelate::AssetMetadata AssetManager::s_NullMetadata;

	std::unordered_map<AssetHandle, Ref<Asset>> AssetManager::s_LoadedAssets;


	std::vector<Pixelate::Ref<Pixelate::Asset>> AssetManager::s_AssetsToBeReloaded;
	bool AssetManager::s_ShouldReloadAssetList;

	std::string AssetManager::m_FilterBuffer;
	bool AssetManager::m_IsFiltering;

	Pixelate::FileWatcherCallback AssetManager::s_Callback;

	

	void AssetManager::Init()
	{
		PX_PROFILE_FUNCTION();
		s_NullMetadata.Type = AssetType::None;

		FileSystem::SetFileWatcherCallback(AssetManager::OnFileWatcherAction);

		s_AssetRegistry.Deserialize();
		ReloadAssets();
		s_AssetRegistry.Serialize();
		

	}

	void AssetManager::Shutdown()
	{
		PX_PROFILE_FUNCTION();
		s_AssetRegistry.Serialize();
		s_AssetRegistry.GetRegistry().clear();
		s_LoadedAssets.clear();
	}

	void AssetManager::OnFileWatcherAction(FileWatcherCallbackData data)
	{

		s_Callback(data);
		if (!data.IsDirectory) {
			switch (data.Action)
			{
				case FileSystemAction::Added: ImportAsset(data.Filepath); break;
				case FileSystemAction::Deleted: RemoveAssetFromRegistry(data.Filepath); break;
				case FileSystemAction::Modified: ReloadAsset(data.Filepath); break;
				case FileSystemAction::Renamed: OnAssetRenamed(data.OldFilepath, data.Filepath); break;
			}
		}
	}

	AssetMetadata& AssetManager::GetMetadata(AssetHandle handle)
	{
		PX_PROFILE_FUNCTION();
		for (auto& [filepath, metadata] : s_AssetRegistry.GetRegistry()) {
			if (metadata.Handle == handle)
				return metadata;
		}

		return s_NullMetadata;
	}

	AssetMetadata& AssetManager::GetMetadata(const std::filesystem::path& path)
	{
		PX_PROFILE_FUNCTION();
		for (auto& [filepath, metadata] : s_AssetRegistry.GetRegistry()) {
			if (filepath == path)
				return metadata;
		}

		return s_NullMetadata;
	}

	AssetHandle AssetManager::ImportAsset(const std::filesystem::path& filepath) {

		PX_PROFILE_FUNCTION();
		auto path = std::filesystem::relative(filepath, s_AssetPath); // should be the asset directory

		if (FileSystem::IsDirectory(filepath))
			return s_NullMetadata.Handle;

		if (s_AssetRegistry.GetRegistry().find(path) != s_AssetRegistry.GetRegistry().end()) {
			return s_AssetRegistry.GetRegistry()[path].Handle;
		}


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
		PX_PROFILE_FUNCTION();
		ProcessDirectoryWhenReloading(s_AssetPath);

	}



	void AssetManager::ProcessDirectoryWhenReloading(const std::filesystem::path& dir)
	{
		PX_PROFILE_FUNCTION();
		for (auto& path : std::filesystem::directory_iterator(dir)) {

			if (path.is_directory())
				ProcessDirectoryWhenReloading(path.path());
			else
				ImportAsset(path.path());

		}
	}


	void AssetManager::ReloadAsset(const std::filesystem::path& filepath)
	{

		if (FileSystem::IsDirectory(filepath))
			return;

		auto path = std::filesystem::relative(filepath, s_AssetPath);
		auto& metadata = s_AssetRegistry.GetRegistry()[path];
		if (metadata.IsLoaded) {
			s_AssetsToBeReloaded.push_back(s_LoadedAssets[metadata.Handle]);
			s_ShouldReloadAssetList = true;
		}


	}


	//TODO: What about assets that are already loaded in the scene? -- should update and handle them
	void AssetManager::RemoveAssetFromRegistry(const std::filesystem::path& filepath)
	{
		auto path = std::filesystem::relative(filepath, s_AssetPath);
		s_AssetRegistry.GetRegistry().erase(filepath);

	}


	void AssetManager::OnAssetRenamed(const std::filesystem::path& oldPath, const std::filesystem::path& newPath) {
		auto oPath = std::filesystem::relative(oldPath, s_AssetPath);
		auto nPath = std::filesystem::relative(newPath, s_AssetPath);

		AssetMetadata metadata = s_AssetRegistry.GetRegistry()[oPath];
		metadata.Filepath = nPath;
		s_AssetRegistry.GetRegistry().erase(oPath);
		s_AssetRegistry.GetRegistry().insert({ nPath, metadata });

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
		if (!m_FilterBuffer.empty())
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


	void AssetManager::ApplyAssetChanges()
	{
		if (s_ShouldReloadAssetList) {
			s_ShouldReloadAssetList = false;

			for (auto& asset : s_AssetsToBeReloaded) {

				if (asset->GetType() == AssetType::Texture) {
					Ref<Texture> texture = std::dynamic_pointer_cast<Texture>(s_LoadedAssets[asset->Handle]);
					texture->Reload();
				}
			}

			s_AssetsToBeReloaded.clear();

		}
	}

}