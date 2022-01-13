#pragma once


#include <string>
#include "API/Shader/Shader.h"
#include "API/Buffer.h"


namespace Pixelate
{
	class Material
	{
		public :
			Material(const Ref<Shader>& shader, const std::string& name) 
				: m_Shader(shader), m_Name(name)
			{
				for (auto& resource : m_Shader->GetResources())
				{
					for (auto& uniformBuffer : resource.Uniforms)
					{
						for (auto& member : uniformBuffer.Members)
						{
							m_UBOs[member].push_back(UniformBuffer::Create(uniformBuffer.StructSize, uniformBuffer.Binding));
						}
					}

				}
			}

			const std::string& GetName() const { return m_Name; }

			void Bind() 
			{
				for (auto& [member, ubo] : m_UBOs)
				{
					for (auto& buffer : ubo)
					{
						buffer->Bind();
					}
				}

				m_Shader->Bind();
			}

			void UnBind()
			{
				for (auto& [member, ubo] : m_UBOs)
				{
					for (auto& buffer : ubo)
					{
						buffer->UnBind();
					}
				}

				m_Shader->Unbind();
			}

			void Set(const std::string& name, int value, int binding = 0)
			{
				auto&& [buffer, member] = FindUniformStorage(name, binding);
				buffer->SetData(&value, member.Size, member.Offset);
			}


		private :
			std::pair<Ref<UniformBuffer>, ShaderMember> FindUniformStorage(const std::string& name, int binding);
		private :
			Ref<Shader> m_Shader;
			std::unordered_map<ShaderMember, std::vector<Ref<UniformBuffer>>> m_UBOs;
			std::string m_Name;
	};
}