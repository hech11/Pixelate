#include "PXpch.h"
#include "GLShader.h"

#include <glad/glad.h>

#include <GLM/glm/gtc/type_ptr.hpp>
#include "Pixelate/Core/Application.h"

#include "GLCommon.h"

#include "Pixelate/Rendering/Renderer2D.h"

#include <VulkanSDK/include/Vulkan/spirv_cross/spirv_glsl.hpp>
#include <VulkanSDK/include/Vulkan/spirv_cross/spirv_cross.hpp>
#include <VulkanSDK/include/Vulkan/shaderc/shaderc.hpp>


namespace Pixelate {


	static std::string FromShaderTypeToString(uint32_t type)
	{
		switch (type)
		{
			case GL_VERTEX_SHADER: return "vertex";
			case GL_FRAGMENT_SHADER: return "fragment";
		}
	}


	GLShader::GLShader(const std::string& filepath)
	{
		std::string source = FileSystem::ReadText(filepath);

		m_Name = std::filesystem::path(filepath).filename().string();

		ParseSources(source);
		CompileVulkanIntoSpirV();
		CompileSpirvIntoGLSL();
		CreateProgram();


	}

	GLShader::GLShader(const std::string& name, const char* source) 
	{
		m_Name = name;

		ParseSources(source);
		CreateProgram();

	}


	GLShader::~GLShader() {
		glDeleteProgram(m_RendererID);
	}

	void GLShader::Bind() const {
		GLCall(glUseProgram(m_RendererID));
	}
	void GLShader::Unbind() const {
		GLCall(glUseProgram(0));
	}



	void GLShader::CompileVulkanIntoSpirV()
	{
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);

		//options.SetOptimizationLevel(shaderc_optimization_level_performance);

		{
			shaderc::SpvCompilationResult results = compiler.CompileGlslToSpv(m_OpenGLSources[GL_VERTEX_SHADER], shaderc_glsl_vertex_shader, m_Name.c_str(), options);

			if (results.GetCompilationStatus() == shaderc_compilation_status_compilation_error)
			{
				PX_CORE_ERROR("%s\n", results.GetErrorMessage().c_str());
			}

			m_SpirvShaderData[GL_VERTEX_SHADER] = std::vector<uint32_t>(results.cbegin(), results.cend());
		}


		{
			shaderc::SpvCompilationResult results = compiler.CompileGlslToSpv(m_OpenGLSources[GL_FRAGMENT_SHADER], shaderc_glsl_fragment_shader, m_Name.c_str(), options);

			if (results.GetCompilationStatus() == shaderc_compilation_status_compilation_error)
			{
				PX_CORE_ERROR("%s\n", results.GetErrorMessage().c_str());
			}

			m_SpirvShaderData[GL_FRAGMENT_SHADER] = std::vector<uint32_t>(results.cbegin(), results.cend());

		}
	}

	void GLShader::CompileSpirvIntoGLSL()
	{
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);

		//options.SetOptimizationLevel(shaderc_optimization_level_performance);
		m_OpenGLSources.clear();

		{
			spirv_cross::CompilerGLSL glslCompiler(m_SpirvShaderData[GL_VERTEX_SHADER]);
			m_OpenGLSources[GL_VERTEX_SHADER] = glslCompiler.compile();
		}

		{
			spirv_cross::CompilerGLSL glslCompiler(m_SpirvShaderData[GL_FRAGMENT_SHADER]);
			m_OpenGLSources[GL_FRAGMENT_SHADER] = glslCompiler.compile();
		}
	}

	void GLShader::ParseSources(const std::string& source)
	{
		enum class ShaderType {
			None = -1,
			Vertex, Fragment
		};

		ShaderType type = ShaderType::None;
		std::string line;
		std::istringstream file(source);
		std::stringstream ss[2];

		while (getline(file, line)) {
			if (line.find("#shader") != std::string::npos) {
				if (line.find("vertex") != std::string::npos) {
					type = ShaderType::Vertex;
				}
				else if (line.find("fragment") != std::string::npos) {
					type = ShaderType::Fragment;
				}
			}
			else {
				ss[(int)type] << line << "\n";
			}
		}

		
		m_OpenGLSources[GL_VERTEX_SHADER] = ss[0].str();
		m_OpenGLSources[GL_FRAGMENT_SHADER] = ss[1].str();

	}

	void GLShader::CreateProgram()
	{
		uint32_t program = glCreateProgram();
		std::vector<uint32> shaders;

		for (auto& [type, source] : m_OpenGLSources)
		{
			shaders.push_back(CreateShader(type, source));
		}

		for (int shader : shaders)
		{
			glAttachShader(program, shader);
		}

		int linked;
		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if (!linked)
		{
			int logLength;
			glGetShaderiv(program, GL_INFO_LOG_LENGTH, &logLength);
			std::vector<char> message(logLength);
			
			glGetProgramInfoLog(program, logLength, &logLength, message.data());

			PX_ERROR("[OpenGL] Shader linking failed (%s):\n(%s)", m_Name.c_str(), message.data());
			glDeleteProgram(program);

			for (auto shader : shaders)
			{
				glDeleteShader(shader);
			}

		}

		for (auto shader : shaders)
		{
			glDetachShader(program, shader);
			glDeleteShader(shader);
		}

		m_RendererID = program;
	}


	uint32_t GLShader::CreateShader(unsigned int type, const std::string& shaderSource) {


		unsigned int shader = glCreateShader(type);
		const char* shdSrc = shaderSource.c_str();
		
		GLCall(glShaderSource(shader, 1, &shdSrc, nullptr));
		GLCall(glCompileShader(shader));

		int validation;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &validation);

		if (!validation) {

			int logLength;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

			std::vector<char> message(logLength);
			glGetShaderInfoLog(shader, logLength, &logLength, message.data());

			std::string shaderType = FromShaderTypeToString(type);
			PX_ERROR("OpenGL (%s) Error! MESSAGE :\n %s\n\n", shaderType.c_str(), message.data());

			GLCall(glDeleteShader(shader));
			return 0;
		}

		return shader;
	}




	int GLShader::GetUniformLocation(const std::string& name) {

		if (m_CachedUniformLocations.find(name) != m_CachedUniformLocations.end()) {
			return m_CachedUniformLocations[name];
		}
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1) {
			PX_CORE_WARN("Uniform '%s' does not exist!", name.c_str());
		}
		m_CachedUniformLocations[name] = location;
		return location;
		
	}



	void GLShader::SetUniform1f(const std::string& uniformName, const float value) {
		glUniform1f(GetUniformLocation(uniformName), value);
	}


	void GLShader::SetUniform2f(const std::string& uniformName, const glm::vec2& values) {
		glUniform2f(GetUniformLocation(uniformName), values.x, values.y);
	}


	void GLShader::SetUniform3f(const std::string& uniformName, const glm::vec3& values) {
		glUniform3f(GetUniformLocation(uniformName), values.x, values.y, values.z);
	}

	void GLShader::SetUniform4f(const std::string& uniformName, const glm::vec4& values) {
		glUniform4f(GetUniformLocation(uniformName), values.x, values.y, values.z, values.w);
	}



	void GLShader::SetUniform1i(const std::string& uniformName, const int value) {
		glUniform1i(GetUniformLocation(uniformName), value);	
	}

	void GLShader::SetUniform1iArray(const std::string& uniformName, unsigned int count, const int* values) {
		glUniform1iv(GetUniformLocation(uniformName), count, values);
	}

	void GLShader::SetUniform2i(const std::string& uniformName, const glm::i32vec2& values) {
		glUniform2i(GetUniformLocation(uniformName), values.x, values.y);
	}


	void GLShader::SetUniform3i(const std::string& uniformName, const glm::i32vec3& values) {
		glUniform3i(GetUniformLocation(uniformName), values.x, values.y, values.z);
	}


	void GLShader::SetUniform4i(const std::string& uniformName, const glm::i32vec4& values) {
		glUniform4i(GetUniformLocation(uniformName), values.x, values.y, values.z, values.z);
	}



	void GLShader::SetUniform1i(const std::string& uniformName, const char value) {
		glUniform1i(GetUniformLocation(uniformName), value);
	}


	void GLShader::SetUniform2i(const std::string& uniformName, const glm::i8vec2& values) {
		glUniform2i(GetUniformLocation(uniformName), values.x, values.y);
	}


	void GLShader::SetUniform3i(const std::string& uniformName, const glm::i8vec3& values) {
		glUniform3i(GetUniformLocation(uniformName), values.x, values.y, values.z);
	}


	void GLShader::SetUniform4i(const std::string& uniformName, const glm::i8vec4& values) {
		glUniform4i(GetUniformLocation(uniformName), values.x, values.y, values.z, values.z);
	}


	void GLShader::SetUniformMatrix(const std::string& uniformName, const glm::mat4& matrix) {
		glUniformMatrix4fv(GetUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(matrix));
	}



}