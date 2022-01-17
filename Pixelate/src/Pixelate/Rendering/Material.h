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

			const std::string& GetName() const { return m_Name; }
			const Ref<Shader>& GetShader() const { return m_Shader; }

			void Bind();
			void UnBind();


			void AddUniformBufferEntry(const ShaderUniform& table);
			void SetShader(const Ref<Shader>& shader);


			const std::vector<MaterialUniformTable>& GetUniformTable() const { return m_UniformTable; }
			std::vector<MaterialUniformTable>& GetUniformTable() { return m_UniformTable; }

			void UpdateMaterial();

			template<typename T>
			void Set(const std::string& name, T value, int binding = 0)
			{
				const auto&& [buffer, member] = FindUniformStorage(name, binding);
				auto& table = FindMaterialTable(binding);

				memcpy((char*)table.Data + member.Offset, &value, member.Size);
				buffer->SetData(&value, member.Size, member.Offset);
			}

			template<typename T>
			T Get(const std::string& name, int binding = 0)
			{
				const auto&& [buffer, member] = FindUniformStorage(name, binding);
				auto& table = FindMaterialTable(binding);

				return *(T*)((char*)table.Data + member.Offset);
			}

			


		private :
			std::pair<Ref<UniformBuffer>, ShaderMember> FindUniformStorage(const std::string& name, int binding = 0);
			MaterialUniformTable& FindMaterialTable(int binding = 0);
		private :
			Ref<Shader> m_Shader;
			std::vector<MaterialUniformTable> m_UniformTable;
			std::string m_Name;
	};
}