#pragma once

#include "Pixelate/Asset/Embedded/Embedded.h"

namespace Pixelate {

	class AudioMixerGenerator : public EmbeddedGenerator {

		public:
			void GenerateToDisk(std::filesystem::path path) override;

	};

}