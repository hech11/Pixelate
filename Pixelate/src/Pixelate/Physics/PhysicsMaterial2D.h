#pragma once


#include "Pixelate/Asset/Asset.h"
#include <filesystem>


namespace Pixelate {

	struct PhysicsMaterial2D : public Asset {


		PhysicsMaterial2D() {
			Handle = 0;
		}

		float Mass = 0.0f;
		float Friction = 0.2f;
		float Bounciness = 0.0f;

		SETUP_ASSET_PROPERTIES(AssetType::PhysicsMaterial2D);

	};


	//struct?
	class PhysicsMaterial2DSerialization {
		public :
			static void Serialize(const std::filesystem::path& path, const Ref<PhysicsMaterial2D>& material);
			static Ref<PhysicsMaterial2D> Deserialize(const std::filesystem::path& path);

	};

}