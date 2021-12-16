#include "PXpch.h"
#include "GLShader.h"

#include <glad/glad.h>

#include <GLM/glm/gtc/type_ptr.hpp>
#include "Pixelate/Core/Application.h"

#include "GLCommon.h"

#include "Pixelate/Rendering/Renderer2D.h"

#include <VulkanSDK/include/Vulkan/spirv_cross/spirv_glsl.hpp>
#include <VulkanSDK/include/Vulkan/spirv_cross/spirv_reflect.hpp>
#include <VulkanSDK/include/Vulkan/spirv_cross/spirv_cross.hpp>
#include <VulkanSDK/include/Vulkan/shaderc/shaderc.hpp>

#include <filesystem>
#include "Pixelate/Utility/FileSystem.h"

namespace Pixelate {


	static std::string FromShaderTypeToString(uint32_t type)
	{
		switch (type)
		{
			case GL_VERTEX_SHADER: return "vertex";
			case GL_FRAGMENT_SHADER: return "fragment";
		}
	}

	static shaderc_shader_kind FromOpenGLShaderTypeToSPIRV(uint32_t type)
	{
		switch (type)
		{
			case GL_VERTEX_SHADER:	return shaderc_glsl_vertex_shader;
			case GL_FRAGMENT_SHADER:	return shaderc_glsl_fragment_shader;
		}
	}

	std::string GLShader::DeduceSPIRVCachedFileExtention(uint32_t type)
	{
		switch (type)
		{
			case GL_VERTEX_SHADER: return ".cached_spirv.pxvert";
			case GL_FRAGMENT_SHADER: return ".cached_spirv.pxfrag";
		}
	}

	std::string GLShader::DeduceOpenGLCachedFileExtention(uint32_t type)
	{
		switch (type)
		{
			case GL_VERTEX_SHADER: return ".cached_opengl.pxvert";
			case GL_FRAGMENT_SHADER: return ".cached_opengl.pxfrag";
		}
	}





	GLShader::GLShader(const std::string& filepath)
	{
		std::string source = FileSystem::ReadText(filepath);

		m_Name = std::filesystem::path(filepath).stem().string();
		FileSystem::CreateDir("cache");



		ParseSources(source);
		CompileVulkanIntoSpirV();
		CompileSpirvIntoGLSL();
		CreateProgram();

	}

	GLShader::GLShader(const std::string& name, const char* source) 
	{
		m_Name = name;

		FileSystem::CreateDir("cache");

		ParseSources(source);
		CompileVulkanIntoSpirV();
		CompileSpirvIntoGLSL();
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

		options.SetOptimizationLevel(shaderc_optimization_level_performance);

		m_VulkanSpirVData.clear();
		for (auto&& [type, source] : m_OpenGLSources)
		{
			std::filesystem::path cacheDir = "cache";
			std::filesystem::path cachedPath = cacheDir / std::string(m_Name + DeduceSPIRVCachedFileExtention(type));

			std::ifstream ifs(cachedPath, std::ios::in | std::ios::binary);
			if (ifs.is_open())
			{
				ifs.seekg(0, std::ios::end);
				auto size = ifs.tellg();
				ifs.seekg(0, std::ios::beg);

				auto& src = m_VulkanSpirVData[type];

				src.resize(size/sizeof(uint32_t));
				ifs.read((char*)src.data(), size);
				ifs.close();

			}
			else
			{
				shaderc::SpvCompilationResult results = compiler.CompileGlslToSpv(m_OpenGLSources[type], FromOpenGLShaderTypeToSPIRV(type), m_Name.c_str());

				if (results.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					PX_CORE_ERROR("%s\n", results.GetErrorMessage().c_str());
				}

				m_VulkanSpirVData[type] = std::vector<uint32_t>(results.cbegin(), results.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& src = m_VulkanSpirVData[type];
					out.write((char*)src.data(), src.size()*sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}
	
	}

	void GLShader::CompileSpirvIntoGLSL()
	{
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		options.SetOptimizationLevel(shaderc_optimization_level_performance);



		m_OpenGLSources.clear();
		m_OpenGLSpirVData.clear();
		for (auto&& [type, source] : m_VulkanSpirVData)
		{
			std::filesystem::path cacheDir = "cache";
			std::filesystem::path cachedPath = cacheDir / std::string(m_Name + DeduceOpenGLCachedFileExtention(type));

			std::ifstream ifs(cachedPath, std::ios::in | std::ios::binary);
			if (ifs.is_open())
			{
				ifs.seekg(0, std::ios::end);
				auto size = ifs.tellg();
				ifs.seekg(0, std::ios::beg);

				auto& src = m_OpenGLSpirVData[type];

				spirv_cross::CompilerGLSL glslCompiler(source);
				m_OpenGLSources[type] = glslCompiler.compile();

				src.clear();

				src.resize(size / sizeof(uint32_t));
				ifs.read((char*)src.data(), size);
				ifs.close();
			}
			else
			{
				spirv_cross::CompilerGLSL glslCompiler(source);
				m_OpenGLSources[type] = glslCompiler.compile();

				shaderc::SpvCompilationResult results = compiler.CompileGlslToSpv(m_OpenGLSources[type], FromOpenGLShaderTypeToSPIRV(type), m_Name.c_str());

				if (results.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					PX_CORE_ERROR("%s\n", results.GetErrorMessage().c_str());
				}

				m_OpenGLSpirVData[type] = std::vector<uint32_t>(results.cbegin(), results.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& src = m_OpenGLSpirVData[type];
					out.write((char*)src.data(), src.size()*sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}

		for (auto& data : m_OpenGLSpirVData)
		{
			Reflect(data.first, data.second);
		}
		
	}



	void GLShader::Reflect(uint32_t type, const std::vector<uint32_t>& shaderData)
	{
		spirv_cross::Compiler compiler(shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();
		std::cout << "\n\n";
		PX_CORE_MSG("Shader reflection: %s [%s]\n", m_Name.c_str(), FromShaderTypeToString(type).c_str());
		PX_CORE_MSG("Uniform buffers: %d\n", resources.uniform_buffers.size());
		PX_CORE_MSG("Sampler size: %d\n", resources.separate_images.size());

		for (auto& resource : resources.uniform_buffers)
		{
			auto& id = compiler.get_type(resource.type_id);
			uint32_t structSize = compiler.get_declared_struct_size(id);
			uint32_t memberSize = id.member_types.size();
			std::cout << "\n";
			PX_CORE_MSG("Resource %s\n", resource.name.c_str());
			PX_CORE_MSG("Struct size %d\n", structSize);
			PX_CORE_MSG("Member size %d\n", memberSize);

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


#if 1
		for (auto&& [type, source] : m_OpenGLSources)
		{
			unsigned int shader = shaders.emplace_back(glCreateShader(type));
			const char* shdSrc = source.c_str();

			GLCall(glShaderSource(shader, 1, &shdSrc, nullptr));
			GLCall(glCompileShader(shader));

			glAttachShader(program, shader);

		}
#else
		for (auto&& [type, source] : m_OpenGLSpirVData)
		{
			uint32_t id = shaders.emplace_back(glCreateShader(type));

 			glShaderBinary(1, &id, GL_SHADER_BINARY_FORMAT_SPIR_V, source.data(), source.size()*sizeof(uint32_t));
 
 			glSpecializeShader(id, "main", 0, nullptr, nullptr);
			glAttachShader(program, id);

		}
#endif


		int linked;
		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		/*if (!linked)
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
		}*/

		m_RendererID = program;
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