#pragma once


namespace RGF {


	enum class ShaderUnifromType {
		None = -1,
		Float4,
		Float3,
		Float2,
		Float,
		Char4,
		Char3,
		Char2,
		Char,
		Int4,
		Int3,
		Int2,
		Int,
		Mat4
	};


	struct ShaderUniform {

		std::string Name;
		ShaderUnifromType Type;
		int Location = -1;

		ShaderUniform(const std::string& name, const ShaderUnifromType& type)
			: Name(name), Type(type)
		{
		}
		virtual int* GetIntData() { return nullptr; }

	};



	class ShaderUniformInt : public ShaderUniform {
	public:
		ShaderUniformInt(const std::string& name, const ShaderUnifromType& type, int data)
			: ShaderUniform(name, type), m_Data(data)
		{
		}


		int* GetIntData() override { return &m_Data; }
	private:
		int m_Data = 0;
	};
}

