#include "PXpch.h"
#include "AssetManager.h"

#include <imgui.h>


namespace Pixelate {





	static std::filesystem::path s_AssetPath = "assets"; //TODO: add to a "project" class

	void AssetManager::Init()
	{
		ReloadAssets();
	}

	void AssetManager::Shutdown()
	{
		s_AssetRegistry.GetRegistry().clear();
	}

	AssetHandle AssetManager::ImportAsset(const std::filesystem::path& filepath) {

		auto path = std::filesystem::relative(filepath, s_AssetPath); // should be the asset directory

		// TODO: If we already have this asset then theres no need to reimport it again.
		// if(has asset already...)


		AssetMetadata metadata;
		metadata.Handle = AssetHandle();
		metadata.Filepath = path;
		metadata.Type = AssetType::Asset; // TODO: conversion function

		s_AssetRegistry.GetRegistry()[metadata.Filepath] = metadata;


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

			ImGui::Text("Handle: %d", metadata.Handle);
			ImGui::Text("Filepath: %s", metadata.Filepath.string().c_str());
			//ImGui::Text("Type: %s", metadata.Type);
			ImGui::Separator();

		}

		ImGui::End();
	}


}