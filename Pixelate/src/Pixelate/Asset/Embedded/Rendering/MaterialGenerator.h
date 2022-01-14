#pragma once


#include "Pixelate/Core/Core.h"
#include "Pixelate/Asset/Embedded/Embedded.h"
#include "Pixelate/Rendering/Material.h"


namespace Pixelate {


	class MaterialGenerator : public EmbeddedGenerator {
		public:
			void GenerateToDisk(std::filesystem::path path) override;

	};

}