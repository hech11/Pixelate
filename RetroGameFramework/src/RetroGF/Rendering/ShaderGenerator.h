#pragma once


#include "RetroGF/Core.h"
#include "RetroGF/Rendering/API/Shader.h"

namespace RGF {


	class RGF_API ShaderGenerator {
		public :

			static ShaderGenerator* GetInstance() { return s_Instance; }
			ShaderGenerator();
			~ShaderGenerator();

			Shader* DefaultShader();
			Shader* TexturedShader();

		private :
			static ShaderGenerator* s_Instance;
	};
}