#pragma once


#include "RetroGF\Core.h"
#include "Shader.h"


namespace RGF {


	class RGF_API ShaderGenerator {
		public:

			static ShaderGenerator* GetInstance() { return s_Instance; }
			ShaderGenerator();
			~ShaderGenerator();

			Ref<Shader> DefaultShader();

		private:
			static ShaderGenerator* s_Instance;
	};

}