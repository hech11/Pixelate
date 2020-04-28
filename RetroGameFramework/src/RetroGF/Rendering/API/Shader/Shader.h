#pragma once

#include "RetroGF/Core/Core.h"
#include "RGFpch.h"
#include "GLM/glm/glm.hpp"


// Shader interface.
// The 'Create' method will decide depending on the API choice. OpenGL, Directx 11 or 12, vulkan etc.


// TODO : Shader and Material do not support UBO's! At some point i need to implement this!

namespace RGF {



	class RGF_API Shader {

		public :
			Shader();
			virtual ~Shader(){}


			virtual void Init() = 0;
			virtual void ShutDown() = 0;

			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;

			virtual void LoadFromFile(const std::string& filepath) = 0;
			virtual void LoadFromSrc(const char* data) = 0;

			virtual void SetUniform1f(const std::string& uniformName, const float values) = 0;
			virtual void SetUniform2f(const std::string& uniformName, const glm::vec2& values) = 0;
			virtual void SetUniform3f(const std::string& uniformName, const glm::vec3& values) = 0;
			virtual void SetUniform4f(const std::string& uniformName, const glm::vec4& values) = 0;


			virtual void SetUniform1i(const std::string& uniformName, const int value) = 0;
			virtual void SetUniform1iArray(const std::string& uniformName, unsigned int count, const int* values) = 0;
			virtual void SetUniform2i(const std::string& uniformName, const glm::i32vec2& values) = 0;
			virtual void SetUniform3i(const std::string& uniformName, const glm::i32vec3& values) = 0;
			virtual void SetUniform4i(const std::string& uniformName, const glm::i32vec4& values) = 0;

			virtual void SetUniform1i(const std::string& uniformName, const char value) = 0;
			virtual void SetUniform2i(const std::string& uniformName, const glm::i8vec2& values) = 0;
			virtual void SetUniform3i(const std::string& uniformName, const glm::i8vec3& values) = 0;
			virtual void SetUniform4i(const std::string& uniformName, const glm::i8vec4& values) = 0;



			virtual void SetUniformMatrix(const std::string& uniformName, const glm::mat4& matrix) = 0;


			// TEMP: This may change when a proper shader parser system is in place
			virtual int GetUniformLocation(const std::string& name) = 0;


		public :
			static Ref<Shader> Create();


	};
	
	


}