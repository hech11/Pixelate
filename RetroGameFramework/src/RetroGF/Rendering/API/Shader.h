#pragma once

#include "RetroGF/Core.h"
#include "RGFpch.h"
#include "GLM/glm/glm.hpp"

// Shader interface.

// The 'Create' method will decide depending on the API choice should Shader choose
// OpenGL, Directx 11 or 12, vulkan etc... implementation of Shader.



namespace RGF {


	class RGF_API Shader {

		public :
			Shader(){}
			virtual ~Shader(){}


			virtual void Init() = 0;
			virtual void ShutDown() = 0;

			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;

			virtual void LoadFromSrc(const std::string& filepath) = 0;

			virtual void SetUniformMatrix(const std::string& uniformName, const glm::mat4& matrix) = 0;
		public :
			static Shader* Create();

	};

}