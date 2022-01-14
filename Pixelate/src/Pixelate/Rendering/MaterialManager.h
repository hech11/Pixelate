#pragma once

#include "Material.h"


#include "Pixelate/Utility/FileSystem.h"

namespace Pixelate
{

	class MaterialManager
	{
		public :
			static Ref<Material> Load(const std::string& filepath);
			static Ref<Material> LoadExternalResource(const std::string& filepath);

			static std::vector<Ref<Material>> & Get() { return s_Materials; }

		private:
			static std::vector<Ref<Material>> s_Materials;
	};

	class MaterialSerialization
	{
		public :
			static void Serialize(const std::filesystem::path& path, const Ref<Material>& material);
			static Ref<Material> Deserialize(const std::filesystem::path& path);
	};

}