#pragma once


#include "Pixelate/Core/Core.h"
#include "Pixelate/Asset/Embedded/Embedded.h"
#include "Pixelate/Rendering/API/Shader/Shader.h"


namespace Pixelate {


	class ShaderGenerator : public EmbeddedGenerator {
		public:
			void GenerateToDisk(std::filesystem::path path) override;

	};

}