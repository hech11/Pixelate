#include "PXpch.h"

#include "ShaderLibrary.h"
#include "Pixelate/Asset/AssetManager.h"


namespace Pixelate
{

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		Ref<Shader> result = AssetManager::GetAsset<Shader>(filepath);
		m_Shaders[name] = result;

		return result;
	}

	Ref<Shader> ShaderLibrary::LoadExternalResource(const std::string& name, const std::string& filepath)
	{
		Ref<Shader> result = Shader::Create(filepath);
		m_Shaders[name] = result;

		return result;
	}

}