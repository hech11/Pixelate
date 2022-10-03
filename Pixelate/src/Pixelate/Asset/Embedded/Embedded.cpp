#include "PXpch.h"

#include "Audio/AudioMixerGenerator.h"
#include "Physics/PhysicsMaterial2DGenerator.h"

#include "Rendering/ShaderGenerator.h"
#include "Rendering/MaterialGenerator.h"


namespace Pixelate {

	inline static std::unordered_map<AssetType, Ref<EmbeddedGenerator>> s_Generators = 
	{
		{AssetType::AudioMixer, CreateRef<AudioMixerGenerator>()},
		{AssetType::PhysicsMaterial2D, CreateRef<PhysicsMaterial2DGenerator>()},
		{AssetType::Shader, CreateRef<ShaderGenerator>()},
		{AssetType::Material, CreateRef<MaterialGenerator>()},
	};




	void Embedded::GenerateToDisk(AssetType type, std::filesystem::path& path) {
		
		if (s_Generators.find(type) == s_Generators.end()) {
			PX_ASSERT(false, "This type of asset does not exist in the asset generator table!\n");
			return;

		}

		s_Generators[type]->GenerateToDisk(path);

	}

}