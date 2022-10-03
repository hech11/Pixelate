#pragma once

#include "Pixelate/Rendering/API/Shader/Shader.h"

// OpenGL's implementation of 'Shader.h'

namespace Pixelate {


	struct ShaderSources {
		std::string VertexShaderStr;
		std::string FragmentShaderStr;
	};


	class PX_API GLShader : public Shader {

		public:
		
			GLShader(const std::string& filepath);
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

			void SetName(const std::string& name) override { m_Name = name; }
			void SetPath(const std::string& path) override { m_Filepath = path; }

			int GetUniformLocation(const std::string& name) override;
			std::string& GetName() { return m_Name; }

			void Reload() override;


		private :
			void CompileVulkanIntoSpirV();
			void CompileSpirvIntoGLSL();


			void Reflect(uint32_t type, const std::vector<uint32_t>& shaderData);

			std::string DeduceSPIRVCachedFileExtention(uint32_t type);
			std::string DeduceOpenGLCachedFileExtention(uint32_t type);

			void ClearCachedFiles();

			void ParseSources(const std::string& source);
			void CreateProgram();


		private :
			uint32_t m_RendererID;

			std::string m_Name;
			std::string m_Filepath;

			std::unordered_map<uint32_t, std::string> m_OpenGLSources;

			std::unordered_map<uint32_t, std::vector<uint32_t>> m_OpenGLSpirVData;
			std::unordered_map<uint32_t, std::vector<uint32_t>> m_VulkanSpirVData;

			std::unordered_map<std::string, int> m_CachedUniformLocations;



	};

}