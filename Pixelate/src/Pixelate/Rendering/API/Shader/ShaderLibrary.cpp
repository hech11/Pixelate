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
		m_QueuedPaths.push_back(std::make_pair(name, filepath));
		return nullptr;
	}

	Ref<Shader> ShaderLibrary::LoadExternalResource(const std::string& name, const std::string& filepath)
	{
		Ref<Shader> result = Shader::Create(filepath);
		m_Shaders[name] = result;

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

				m_Shaders[old]->SetName(value);
				m_Shaders[old]->SetPath(data.Filepath.string());

				auto node = m_Shaders.extract(old);
				node.key() = value;
				m_Shaders.insert(std::move(node));
				m_Shaders.erase(old);

			};

		}
	}

}