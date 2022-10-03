#include "PXpch.h"

#include "ShaderLibrary.h"


namespace Pixelate
{

	void ShaderLibrary::Init()
	{
		AssetManager::AddAssetCallback(PX_BIND_EVENT_FNC(ShaderLibrary::OnFileWatcherAction));
	}

	Ref<Shader> ShaderLibrary::Add(const std::string& name, const std::string& filepath)
	{
		std::filesystem::path p(filepath);
		if(p.extension() == ".pxShader")
			m_QueuedPaths.push_back(std::make_pair(name, filepath));
		return nullptr;
	}

	Ref<Shader> ShaderLibrary::LoadExternalResource(const std::string& name, const std::string& filepath)
	{
		Ref<Shader> result = Shader::Create(filepath);


		std::filesystem::path childpath = std::filesystem::relative(filepath, "resources");
		result->Handle = AssetManager::GetMetadata(childpath).Handle;
		m_ResourceShaders[name] = result;

		return result;
	}

	void ShaderLibrary::ApplyQueuedChanges()
	{
		if (m_QueuedPaths.size())
		{

			for (auto&& [name, path] : m_QueuedPaths)
			{
				Ref<Shader> result = AssetManager::GetAsset<Shader>(path);
				m_Shaders[name] = result;
			}

			m_QueuedPaths.clear();
		}
	}

	void ShaderLibrary::OnFileWatcherAction(FileWatcherCallbackData data)
	{

		auto path = std::filesystem::relative("assets", data.OldFilepath);
		bool isShader = AssetManager::GetMetadata(path.string()).Type == AssetType::Shader;

		if (isShader)
			return;

		path = std::filesystem::relative("assets", data.Filepath);
		isShader = AssetManager::GetMetadata(path.string()).Type == AssetType::Shader;
		if (isShader)
			return;

		switch (data.Action)
		{

			case FileSystemAction::Added: {
				std::filesystem::path path = data.Filepath;
				path = std::filesystem::relative(path, "assets");
				Add(data.Filepath.stem().string(), path.string()); break;
	
			}
			case FileSystemAction::Renamed: {


				auto old = data.OldFilepath.stem().string();
				auto value = data.Filepath.stem().string();

				auto node = m_Shaders.extract(old);
				node.key() = value;

				m_Shaders.insert(std::move(node));


				m_Shaders[value]->SetName(value);
				m_Shaders[value]->SetPath(data.Filepath.string());
				m_Shaders.erase(old);

			};

		}
	}

}