#pragma once


#include "Pixelate/Core/Core.h"
#include "Pixelate/Asset/Embedded/Embedded.h"
#include "Shader.h"


namespace Pixelate {


	class ShaderGenerator : public EmbeddedGenerator {
		public:
			virtual void GenerateToDisk(std::filesystem::path path) = 0;

	};

}