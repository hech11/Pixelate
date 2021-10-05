#pragma once

#include "Pixelate/Asset/Embedded/Embedded.h"


namespace Pixelate {
	class PhysicsMaterial2DGenerator : public EmbeddedGenerator {

		public:
			void GenerateToDisk(std::filesystem::path path) override;

	};
}