#pragma once

#include "API/Shader/Shader.h"
#include "API/Buffer.h"


namespace Pixelate
{

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
			if (Data)
				delete[] Data;

			Data = nullptr;
			Size = 0;

		}

	private:
		void SetTableData(void* data, uint32_t size)
		{
			if (data)
			{
				if (Data)
				{
					delete[] Data;
				}

				Data = new char[size];
				memcpy(Data, data, size);
				Size = size;
			}
		}
		friend class MaterialSerialization;
	};
}