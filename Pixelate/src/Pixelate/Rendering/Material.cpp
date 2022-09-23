#include "PXpch.h"
#include "Material.h"
#include <utility>


namespace Pixelate
{

	Material::Material(const Ref<Shader>& shader, const std::string& name, bool initBufferTable)
		: m_Shader(shader), m_Name(name)
	{
		Invalidate(shader, name, initBufferTable);
	}


	void Material::Invalidate(const Ref<Shader>& shader, const std::string& name, bool initBufferTable /*= true*/)
	{
		if (initBufferTable)
		{
			for (auto& resource : m_Shader->GetResources())
			{
				for (auto& uniformBuffer : resource.Uniforms)
				{
					Ref<MaterialUniformTable> table = CreateRef<MaterialUniformTable>();

					table->UBO = UniformBuffer::Create(uniformBuffer.StructSize, uniformBuffer.Binding);
					table->Size = uniformBuffer.StructSize;
					table->ReflectedUniformBuffer = uniformBuffer;
					table->InvalidateData();

					m_UniformTable.push_back(table);


				}

			}
		}
	}

	void Material::Bind()
	{
		for (auto&& table : m_UniformTable)
		{
			table->UBO->Bind();
		}

		m_Shader->Bind();
	}

	void Material::UnBind()
	{
		for (auto&& table : m_UniformTable)
		{
			table->UBO->UnBind();
		}

		m_Shader->Unbind();
	}


	void Material::AddUniformBufferEntry(const ShaderUniform& table)
	{

		Ref<MaterialUniformTable> entry = CreateRef<MaterialUniformTable>();
		entry->UBO = UniformBuffer::Create(table.StructSize, table.Binding);
		entry->Size = table.StructSize;
		entry->ReflectedUniformBuffer = table;
		entry->InvalidateData();

		m_UniformTable.push_back(entry);
	}

	void Material::SetShader(const Ref<Shader>& shader)
	{
		m_Shader = shader;
		m_UniformTable.clear();

		Invalidate(shader, m_Name);
		
	}

	void Material::UpdateMaterial()
	{
		for (auto&& table : m_UniformTable)
		{
			table->UBO->SetData(table->Data.data(), table->Size, 0);
		}
	}

	std::pair<Ref<UniformBuffer>, ShaderMember> Material::FindUniformStorage(const std::string& name, int binding)
	{
		if (m_UniformTable.size())
		{
			for (auto& table : m_UniformTable)
			{
				if (table->ReflectedUniformBuffer.Binding == binding)
				{
					for (auto& member : table->ReflectedUniformBuffer.Members)
					{
						if (member.Name == name)
						{
							return std::make_pair(table->UBO, member);
						}
					}
				}
			}
		}

	}


	Ref<Pixelate::MaterialUniformTable>& Material::FindMaterialTable(int binding)
	{
		if (m_UniformTable.size())
		{
			for (auto& table : m_UniformTable)
			{
				if (table->ReflectedUniformBuffer.Binding == binding)
					return table;
			}
		}
	}


	uint32_t Material::GetShaderBaseTypeInBytes(ShaderBaseType type)
	{
		switch (type)
		{
			case Pixelate::ShaderBaseType::None: return -1;
			case Pixelate::ShaderBaseType::Int8: return 1;
			case Pixelate::ShaderBaseType::Int16: return 2;
			case Pixelate::ShaderBaseType::Int32: return 4;
			case Pixelate::ShaderBaseType::UInt32: return 4;
			case Pixelate::ShaderBaseType::Float: return 4;
			case Pixelate::ShaderBaseType::Bool: return 1;
			case Pixelate::ShaderBaseType::Mat4: return -1;
			case Pixelate::ShaderBaseType::SampledImage: return -1;
		}
	}

}