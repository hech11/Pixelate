#pragma once

#include "Pixelate/Rendering/API/Shader/Shader.h"

namespace Pixelate
{
	class ShaderLibrary
	{
		public :
			Ref<Shader> Load(const std::string& name, const std::string& filepath);

			std::unordered_map<std::string, Ref<Shader>>& Get() { return m_Shaders; }

		private :
			std::unordered_map<std::string, Ref<Shader>> m_Shaders;

	};
}