#pragma once

#include "Pixelate/Rendering/API/Shader/Shader.h"

// OpenGL's implementation of 'Shader.h'

namespace Pixelate {

	class PX_API GLShader : public Shader {

		public:
			struct ShaderSources {
				std::string VertexShaderStr;
				std::string FragmentShaderStr;
			};

			GLShader(const std::string& filepath);
			GLShader(const std::string& name, const char* source);

			~GLShader();


			void Bind() const override;
			void Unbind() const override;

			
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

			void ParseSources(const std::string& source);
			void CreateProgram();

			uint32_t CreateShader(unsigned int type, const std::string& shaderSource);

		private :
			uint32_t m_RendererID;

			std::string m_Name;
			std::unordered_map<uint32_t, std::string> m_OpenGLSources;
			std::unordered_map<std::string, int> m_CachedUniformLocations;


	};

}