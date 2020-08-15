#pragma once


#include "Pixelate/Core/Core.h"
#include "Shader.h"


namespace Pixelate {


	class PX_API ShaderGenerator {
		public:

			static ShaderGenerator* GetInstance() { return s_Instance; }
			ShaderGenerator();
			~ShaderGenerator();

			Ref<Shader> DefaultShader();

		private:
			static ShaderGenerator* s_Instance;
	};

}