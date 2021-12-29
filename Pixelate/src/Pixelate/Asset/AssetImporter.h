#pragma once

#include "Asset.h"



namespace Pixelate {



	class AssetTypeImporter {
		public:
			virtual ~AssetTypeImporter() = default;

			virtual bool TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset) = 0;
			virtual void Reload(Ref<Asset>& asset) = 0;
	};

	class AssetImporter {

		public :
			static void Init();
			static void Shutdown();

			static bool TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset);
			static void Reload(Ref<Asset>& asset);

		private :
			static std::unordered_map<AssetType, Ref<AssetTypeImporter>> m_Importers;
	};


	
	class TextureImporter : public AssetTypeImporter {

		public :
			TextureImporter() = default;
			bool TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset) override;
			void Reload(Ref<Asset>& asset) override;


	};



	class AudioImporter : public AssetTypeImporter {

		public:
			AudioImporter() = default;
			bool TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset) override;
			void Reload(Ref<Asset>& asset) override;

	};


	
	class PhysicsMaterial2DImporter : public AssetTypeImporter {

		public:
			PhysicsMaterial2DImporter() = default;
			bool TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset) override;
			void Reload(Ref<Asset>& asset) override;

	};

	class ShaderImporter : public AssetTypeImporter
	{
		public :
			ShaderImporter() = default;
			bool TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset) override;
			void Reload(Ref<Asset>& asset) override;
	};

}