#pragma once

#include "Pixelate/Rendering/API/Shader/Shader.h"
#include "Pixelate/Asset/AssetManager.h"

namespace Pixelate
{
	class ShaderLibrary
	{
		public :
			void Init();

			Ref<Shader> Add(const std::string& name, const std::string& filepath);
			Ref<Shader> LoadExternalResource(const std::string& name, const std::string& filepath);

			std::unordered_map<std::string, Ref<Shader>>& Get() { return m_Shaders; }

			void ApplyQueuedChanges();

		private :
			void OnFileWatcherAction(FileWatcherCallbackData data);

			std::unordered_map<std::string, Ref<Shader>> m_Shaders;
			std::vector<std::pair<std::string, std::string>> m_QueuedPaths;

	};
}