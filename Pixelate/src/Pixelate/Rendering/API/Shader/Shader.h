#pragma once

#include "Pixelate/Core/Core.h"
#include "PXpch.h"
#include "GLM/glm/glm.hpp"

#include "Pixelate/Asset/Asset.h"


// Shader interface.
// The 'Create' method will decide depending on the API choice. OpenGL, Directx 11 or 12, vulkan etc.


// TODO : Shader and Material do not support UBO's! At some point i need to implement this!

namespace Pixelate {


	enum class ShaderBaseType
	{
		None = -1,
		Int8,
		Int16,
		Int32,
		UInt32,
		Float,
		Bool,
		Mat4,
		SampledImage

	};
	struct ShaderMember
	{
		std::string Name;
		uint32_t Size;
		uint32_t Offset;
		ShaderBaseType Type;
	};

	struct ShaderUniform
	{
		uint32_t StructSize;
		uint32_t Binding;
		uint32_t MemberSize;

		std::vector<ShaderMember> Members;
	};
	struct ShaderResource
	{
		std::string Name;

		uint32_t UniformBufferSize;
		uint32_t SampledBufferSize;

		std::vector<ShaderUniform> Uniforms;
	};


	class PX_API Shader : public Asset {

		public :
			Shader();
			virtual ~Shader(){}


			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;


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


			virtual std::vector<ShaderResource>& GetResources() { return m_Resources; }
			virtual std::string& GetName() = 0;


			virtual void Reload() = 0;
			SETUP_ASSET_PROPERTIES(AssetType::Shader);


		public :
			static Ref<Shader> Create(const std::string& filepath);

		protected :
			std::vector<ShaderResource> m_Resources;

	};
	
	


}