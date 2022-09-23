#pragma once

#include "API/Shader/Shader.h"
#include "API/Buffer.h"


namespace Pixelate
{

	struct MaterialUniformTable
	{
		ShaderUniform ReflectedUniformBuffer;
		Ref<UniformBuffer> UBO;

		std::vector<uint8_t> Data;
		uint32_t Size = 0;

		void InvalidateData()
		{
			Data.clear();
			Data.resize(Size);
		}


		MaterialUniformTable()
		{

		}
		MaterialUniformTable(const MaterialUniformTable& other)
		{
			Data.clear();
			Data.resize(Size);

			memcpy(Data.data(), other.Data.data(), other.Size);
			Size = other.Size;
			ReflectedUniformBuffer = other.ReflectedUniformBuffer;
			UBO = other.UBO;
		}
		~MaterialUniformTable()
		{
			if (Data.size())
				Data.clear();

			Size = 0;

		}

	private:
		void SetTableData(void* data, uint32_t size)
		{
			if (data)
			{
				if (Data.size())
					Data.clear();

				Data.resize(size);
				memcpy(Data.data(), data, size);
				Size = size;
			}
		}
		friend class MaterialSerialization;
	};
}