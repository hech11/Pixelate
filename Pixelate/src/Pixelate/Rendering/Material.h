#pragma once


#include <string>
#include "API/Shader/Shader.h"
#include "API/Buffer.h"


namespace Pixelate
{

	// Research: Does each table need it's own instance of the data or should it be stored onto the material?
	struct MaterialUniformTable
	{
		ShaderUniform ReflectedUniformBuffer;
		Ref<UniformBuffer> UBO;

		void* Data = nullptr;
		uint32_t Size = 0;

		void InvalidateData()
		{
			if (Data)
			{
				delete[] Data;
			}

			Data = new char[Size];
		}

		MaterialUniformTable()
		{

		}
		MaterialUniformTable(const MaterialUniformTable& other)
		{
			Data = new char[other.Size];
			memcpy(Data, other.Data, other.Size);
			Size = other.Size;
			ReflectedUniformBuffer = other.ReflectedUniformBuffer;
			UBO = other.UBO;
		}
		~MaterialUniformTable()
		{
			if(Data)
				delete[] Data;

			Data = nullptr;
			Size = 0;

		}
	};

	class Material : public Asset
	{
		public :
			Material(const Ref<Shader>& shader, const std::string& name);

			const std::string& GetName() const { return m_Name; }

			void Bind();
			void UnBind();


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


			void UpdateMaterial();

		private :
			std::pair<Ref<UniformBuffer>, ShaderMember> FindUniformStorage(const std::string& name, int binding = 0);
			MaterialUniformTable& FindMaterialTable(int binding = 0);
		private :
			Ref<Shader> m_Shader;
			std::vector<MaterialUniformTable> m_UniformTable;
			std::string m_Name;
	};
}