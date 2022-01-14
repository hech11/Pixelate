#pragma once

#include "Material.h"


#include "Pixelate/Utility/FileSystem.h"

namespace Pixelate
{

	class MaterialManager
	{
		public :
			Ref<Material> Load(const std::string& filepath);
			Ref<Material> LoadExternalResource(const std::string& filepath);

			std::vector<Ref<Material>> & Get() { return m_Materials; }

		private:
			std::vector<Ref<Material>> m_Materials;
	};

	class MaterialSerialization
	{
		public :
			static void Serialize(const std::filesystem::path& path, const Ref<Material>& material);
			static Ref<Material> Deserialize(const std::filesystem::path& path);
	};

}