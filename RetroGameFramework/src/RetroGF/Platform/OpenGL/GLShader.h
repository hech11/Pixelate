#pragma once

#include "RetroGF/Rendering/API/Shader/Shader.h"

// OpenGL's implementation of 'Shader.h'

namespace RGF {

	class RGF_API GLShader : public Shader {

		public:
			struct ShaderSource {
				std::string VertexShaderStr;
				std::string FragmentShaderStr;
			};

			GLShader();
			~GLShader();

			void Init() override;
			void ShutDown() override;


			void Bind() const override;
			void Unbind() const override;

			void LoadFromFile(const std::string& filepath) override;
			void LoadFromSrc(const char* data) override;


			
			void SetUniform1f(const std::string& uniformName, const float value) override;
			void SetUniform2f(const std::string& uniformName, const glm::vec2& values) override;
			void SetUniform3f(const std::string& uniformName, const glm::vec3& values) override;
			void SetUniform4f(const std::string& uniformName, const glm::vec4& values) override;


			void SetUniform1i(const std::string& uniformName, const int value) override;
			void SetUniform1iArray(const std::string& uniformName, unsigned int count, const int* values) override;
			void SetUniform2i(const std::string& uniformName, const glm::i32vec2& values) override;
			void SetUniform3i(const std::string& uniformName, const glm::i32vec3& values) override;
			void SetUniform4i(const std::string& uniformName, const glm::i32vec4& values) override;

			void SetUniform1i(const std::string& uniformName, const char value) override;
			void SetUniform2i(const std::string& uniformName, const glm::i8vec2& values) override;
			void SetUniform3i(const std::string& uniformName, const glm::i8vec3& values) override;
			void SetUniform4i(const std::string& uniformName, const glm::i8vec4& values) override;

			void SetUniformMatrix(const std::string& uniformName, const glm::mat4& matrix) override;



			int GetUniformLocation(const std::string& name) override;

		private :
			ShaderSource PraseShader(const std::string& shaderFile);
			ShaderSource PraseShader(const char* data);
			unsigned int CreateShader(unsigned int type, const std::string& shaderSource);

		private :
			unsigned int m_RendererID;

			std::string m_Filepath;
			std::unordered_map<std::string, int> m_CachedUniformLocations;


	};

}