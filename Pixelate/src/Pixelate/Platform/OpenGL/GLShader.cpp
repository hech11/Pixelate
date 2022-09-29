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
			case GL_GEOMETRY_SHADER: return "geometry";
		}
	}

	static shaderc_shader_kind FromOpenGLShaderTypeToSPIRV(uint32_t type)
	{
		switch (type)
		{
			case GL_VERTEX_SHADER:	return shaderc_glsl_vertex_shader;
			case GL_FRAGMENT_SHADER:	return shaderc_glsl_fragment_shader;
			case GL_GEOMETRY_SHADER:	return shaderc_glsl_geometry_shader;
		}
	}

	std::string GLShader::DeduceSPIRVCachedFileExtention(uint32_t type)
	{
		switch (type)
		{
			case GL_VERTEX_SHADER: return ".cached_spirv.pxvert";
			case GL_FRAGMENT_SHADER: return ".cached_spirv.pxfrag";
			case GL_GEOMETRY_SHADER: return ".cached_spirv.pxgeo";
		}
	}

	std::string GLShader::DeduceOpenGLCachedFileExtention(uint32_t type)
	{
		switch (type)
		{
			case GL_VERTEX_SHADER: return ".cached_opengl.pxvert";
			case GL_FRAGMENT_SHADER: return ".cached_opengl.pxfrag";
			case GL_GEOMETRY_SHADER: return ".cached_opengl.pxgeo";
		}
	}


	static ShaderBaseType SpirVShaderTypeToPXShaderType(const spirv_cross::SPIRType& type)
	{
		switch (type.basetype)
		{
			case spirv_cross::SPIRType::BaseType::Char: return ShaderBaseType::Int8;
			case spirv_cross::SPIRType::BaseType::Short: return ShaderBaseType::Int16;
			case spirv_cross::SPIRType::BaseType::Int: return ShaderBaseType::Int32;
			case spirv_cross::SPIRType::BaseType::Float:
			{
				if(type.columns == 4)
					return ShaderBaseType::Mat4;

				return ShaderBaseType::Float;
			}
			case spirv_cross::SPIRType::BaseType::SampledImage: return ShaderBaseType::SampledImage;

			default:
				break;
		}
	}

	static uint32_t PXShaderTypeToBytes(const ShaderBaseType& type)
	{
		switch (type)
		{
			case ShaderBaseType::Bool: return 1;
			case ShaderBaseType::Float: return 4;
			case ShaderBaseType::Int16: return 2;
			case ShaderBaseType::Int32: return 4;
			case ShaderBaseType::Mat4: return 4*4*4;
		}
	}



	void GLShader::ClearCachedFiles()
	{

		std::vector<std::string> paths;
		for (auto& data : m_OpenGLSpirVData)
		{
			std::filesystem::path cacheDir = "cache";
			std::filesystem::path cachedPath = cacheDir / std::string(m_Name + DeduceOpenGLCachedFileExtention(data.first));

			std::ifstream ifs(cachedPath, std::ios::in | std::ios::binary);
			if (ifs.is_open())
			{
				paths.push_back(cachedPath.string());
			}
			ifs.close();
		}


		for (auto& data : m_VulkanSpirVData)
		{
			std::filesystem::path cacheDir = "cache";
			std::filesystem::path cachedPath = cacheDir / std::string(m_Name + DeduceSPIRVCachedFileExtention(data.first));

			std::ifstream ifs(cachedPath, std::ios::in | std::ios::binary);
			if (ifs.is_open())
			{
				paths.push_back(cachedPath.string());
			}
			ifs.close();

		}


		for (auto& path : paths)
		{
			FileSystem::DeleteFile(path);
		}
	}

	GLShader::GLShader(const std::string& filepath)
	{
		m_Filepath = filepath;
		std::string source = FileSystem::ReadText(m_Filepath);

		m_Name = std::filesystem::path(m_Filepath).stem().string();
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

		ShaderResource shaderResource;
		shaderResource.Name = std::string(m_Name) + FromShaderTypeToString(type).c_str();
		shaderResource.UniformBufferSize = resources.uniform_buffers.size();
		shaderResource.SampledBufferSize = resources.sampled_images.size();

		// We only support 2D textures for now
		for (auto& sampledImages : resources.sampled_images)
		{
			std::pair<ShaderMember, SampledImage2DContainer> result;

			auto& member = result.first;
			auto& sampledImage = result.second;

			member.Name = sampledImages.name;
			member.Offset = 0;
			member.Size = PXShaderTypeToBytes(member.Type);
			member.Type = SpirVShaderTypeToPXShaderType(compiler.get_type(sampledImages.type_id));


			shaderResource.SampledImage2DContainers.push_back(result);
		}



		for (auto& resource : resources.uniform_buffers)
		{

			ShaderUniform uniform;
			auto& id = compiler.get_type(resource.type_id);
			uniform.StructSize = compiler.get_declared_struct_size(id);
			uniform.Binding = compiler.get_decoration(resource.id, spv::Decoration::DecorationBinding);
			uniform.MemberSize = id.member_types.size();


		
			uint32_t offset = 0;
			for (int i = 0; i < uniform.MemberSize; i++)
			{
				
				ShaderMember member;
				member.Name = compiler.get_member_name(resource.base_type_id, i).c_str();
				member.Type = SpirVShaderTypeToPXShaderType(compiler.get_type(id.member_types[i]));


				member.Offset = offset;

				member.Size = PXShaderTypeToBytes(member.Type);
				offset += member.Size;

				uniform.Members.push_back(member);

			}

			shaderResource.Uniforms.push_back(uniform);

		}

		m_Resources.push_back(shaderResource);

	}


	void GLShader::ParseSources(const std::string& source)
	{
		enum class ShaderType {
			None = -1,
			Vertex, Fragment, Geometry
		};

		ShaderType type = ShaderType::None;
		std::string line;
		std::istringstream file(source);
		std::stringstream ss[3];

		while (getline(file, line)) {
			if (line.find("#shader") != std::string::npos) {
				if (line.find("vertex") != std::string::npos) {
					type = ShaderType::Vertex;
				}
				else if (line.find("fragment") != std::string::npos) {
					type = ShaderType::Fragment;
				}
				else if (line.find("geometry") != std::string::npos) {
					type = ShaderType::Geometry;
				}
			}
			else {
				ss[(int)type] << line << "\n";
			}
		}

		
		m_OpenGLSources[GL_VERTEX_SHADER] = ss[0].str();
		m_OpenGLSources[GL_FRAGMENT_SHADER] = ss[1].str();
		if (ss[2].str().size())
		{
			m_OpenGLSources[GL_GEOMETRY_SHADER] = ss[2].str();
		}
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



	void GLShader::Reload()
	{
		glDeleteProgram(m_RendererID);
		std::string source = FileSystem::ReadText(m_Filepath);

		FileSystem::CreateDir("cache");

		ClearCachedFiles();

		ParseSources(source);
		CompileVulkanIntoSpirV();
		CompileSpirvIntoGLSL();
		CreateProgram();

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