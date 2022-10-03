#pragma once


#include <string>
#include "MaterialUniformTable.h"


namespace Pixelate
{

	class Material : public Asset
	{
		public :
			Material(const Ref<Shader>& shader, const std::string& name, bool initBufferTable = true);

			void Invalidate(const Ref<Shader>& shader, const std::string& name, bool initBufferTable = true);

			void SetName(const std::string& name) { m_Name = name; }
			const std::string& GetName() const { return m_Name; }
			const Ref<Shader>& GetShader() const { return m_Shader; }

			void Bind();
			void UnBind();


			void AddUniformBufferEntry(const ShaderUniform& table);
			void AddSampledImageEntry(int binding, SampledImage2DContainer& container);

			void SetShader(const Ref<Shader>& shader);


			const std::vector<Ref<MaterialUniformTable>>& GetUniformTable() const { return m_UniformTable; }
			std::vector<Ref<MaterialUniformTable>>& GetUniformTable() { return m_UniformTable; }

			std::unordered_map<int, SampledImage2DContainer>& GetReflectedSampledImages() { return m_ReflectedSampledImage2Ds; }
			const std::unordered_map<int, SampledImage2DContainer>& GetReflectedSampledImages() const { return m_ReflectedSampledImage2Ds; }

			void UpdateMaterial();

			template<typename T>
			void Set(const std::string& name, T value, int binding = 0)
			{
				const auto&& [buffer, member] = FindUniformStorage(name, binding);
				auto& table = FindMaterialTable(binding);

				memcpy((uint8_t*)table->Data.data() + member.Offset, &value, member.Size);

				buffer->SetData(table->Data.data(), table->Size, 0);
			}

			template<typename T>
			T Get(const std::string& name, int binding = 0)
			{
				const auto&& [buffer, member] = FindUniformStorage(name, binding);
				auto& table = FindMaterialTable(binding);

				return *(T*)((uint8_t*)table->Data.data() + member.Offset);
			}

			


		private :
			std::pair<Ref<UniformBuffer>, ShaderMember> FindUniformStorage(const std::string& name, int binding = 0);
			Ref<MaterialUniformTable>& FindMaterialTable(int binding = 0);

			uint32_t GetShaderBaseTypeInBytes(ShaderBaseType type);
		private :
			Ref<Shader> m_Shader;
			std::vector<Ref<MaterialUniformTable>> m_UniformTable;
			std::unordered_map<int, SampledImage2DContainer> m_ReflectedSampledImage2Ds;

			std::string m_Name;
	};
}