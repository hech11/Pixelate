#include "PXpch.h"
#include "Material.h"
#include <utility>


namespace Pixelate
{

	Material::Material(const Ref<Shader>& shader, const std::string& name, bool initBufferTable)
		: m_Shader(shader), m_Name(name)
	{
		if (initBufferTable)
		{
			for (auto& resource : m_Shader->GetResources())
			{
				for (auto& uniformBuffer : resource.Uniforms)
				{
					MaterialUniformTable table;

					table.UBO = UniformBuffer::Create(uniformBuffer.StructSize, uniformBuffer.Binding);
					table.Size = uniformBuffer.StructSize;
					table.ReflectedUniformBuffer = uniformBuffer;
					table.InvalidateData();

					m_UniformTable.push_back(table);


				}

			}
		}
	}


	void Material::Bind()
	{
		for (auto&& table : m_UniformTable)
		{
			table.UBO->Bind();
		}

		m_Shader->Bind();
	}

	void Material::UnBind()
	{
		for (auto&& table : m_UniformTable)
		{
			table.UBO->UnBind();
		}

		m_Shader->Unbind();
	}


	void Material::AddUniformBufferEntry(const ShaderUniform& table)
	{

		MaterialUniformTable entry;
		entry.UBO = UniformBuffer::Create(table.StructSize, table.Binding);
		entry.Size = table.StructSize;
		entry.ReflectedUniformBuffer = table;
		entry.InvalidateData();

		m_UniformTable.push_back(entry);
	}

	void Material::UpdateMaterial()
	{
		for (auto&& table : m_UniformTable)
		{
			table.UBO->SetData(table.Data, table.Size, 0);
		}
	}

	std::pair<Ref<UniformBuffer>, ShaderMember> Material::FindUniformStorage(const std::string& name, int binding)
	{
		if (m_UniformTable.size())
		{
			for (auto& table : m_UniformTable)
			{
				if (table.ReflectedUniformBuffer.Binding == binding)
				{
					for (auto& member : table.ReflectedUniformBuffer.Members)
					{
						if (member.Name == name)
						{
							return std::make_pair(table.UBO, member);
						}
					}
				}
			}
		}

	}


	Pixelate::MaterialUniformTable& Material::FindMaterialTable(int binding)
	{
		if (m_UniformTable.size())
		{
			for (auto& table : m_UniformTable)
			{
				if (table.ReflectedUniformBuffer.Binding == binding)
					return table;
			}
		}
	}

}