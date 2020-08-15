#pragma once


namespace Pixelate {


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

		virtual void* ReturnData() { return nullptr; }

	};

	template<typename T>
	class UniformData : public ShaderUniform {
	public:

		template<typename T>
		void CalculateType() {
			PX_ASSERT(false, "Shader type is not listed!");
		}

		template<>
		void CalculateType<int>() {
			Type = ShaderUnifromType::Int;
		}

		template<>
		void CalculateType<glm::i32vec2>() {
			Type = ShaderUnifromType::Int2;
		}

		template<>
		void CalculateType<glm::i32vec3>() {
			Type = ShaderUnifromType::Int3;
		}


		template<>
		void CalculateType<glm::i32vec4>() {
			Type = ShaderUnifromType::Int4;
		}

		template<>
		void CalculateType<char>() {
			Type = ShaderUnifromType::Char;
		}


		template<>
		void CalculateType<glm::i8vec2>() {
			Type = ShaderUnifromType::Char2;
		}


		template<>
		void CalculateType<glm::i8vec3>() {
			Type = ShaderUnifromType::Char3;
		}

		template<>
		void CalculateType<glm::i8vec4>() {
			Type = ShaderUnifromType::Char4;
		}



		template<>
		void CalculateType<float>() {
			Type = ShaderUnifromType::Float;
		}
		template<>
		void CalculateType<glm::vec2>() {
			Type = ShaderUnifromType::Float2;
		}
		template<>
			void CalculateType<glm::vec3>() {
			Type = ShaderUnifromType::Float3;
		}
		template<>
			void CalculateType<glm::vec4>() {
			Type = ShaderUnifromType::Float4;
		}


		template<>
		void CalculateType<glm::mat4>() {
			Type = ShaderUnifromType::Mat4;
		}


		UniformData(const std::string& name, T data)
			: ShaderUniform(name, ShaderUnifromType::None), m_Data(data)
		{
			CalculateType<T>();
		}


		void* ReturnData() override { return &m_Data; }
	private:
		T m_Data;
	};
}

