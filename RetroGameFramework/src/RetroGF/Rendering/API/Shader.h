#pragma once

#include "RetroGF/Core.h"
#include "RGFpch.h"
#include "GLM/glm/glm.hpp"


// Shader interface.
// The 'Create' method will decide depending on the API choice. OpenGL, Directx 11 or 12, vulkan etc.


// TODO : Shader and Material do not support UBO's! At some point i need to implement this!

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
		public :
			ShaderUniformInt(const std::string& name, const ShaderUnifromType& type, int data)
				: ShaderUniform(name, type), m_Data(data)
			{
			}


			int* GetIntData() override { return &m_Data; }
		private :
			int m_Data = 0;
	};


	class RGF_API Shader {

		public :
			Shader();
			virtual ~Shader(){}


			virtual void Init() = 0;
			virtual void ShutDown() = 0;

			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;

			virtual void LoadFromFile(const std::string& filepath) = 0;
			virtual void LoadFromSrc(const char* data) = 0;

			virtual void SetUniform4f(const std::string& uniformName, const glm::vec4& values) = 0;
			virtual void SetUniform1i(const std::string& uniformName, const int value) = 0;
			virtual void SetUniformMatrix(const std::string& uniformName, const glm::mat4& matrix) = 0;


			virtual int GetUniformLocation(const std::string& name) = 0;


		public :
			static Shader* Create();


	};
	
	class RGF_API ShaderGenerator {
		public :

			static ShaderGenerator* GetInstance() { return s_Instance; }
			ShaderGenerator();
			~ShaderGenerator();

			Shader* DefaultShader();
			Shader* TexturedShader();

		private :
			static ShaderGenerator* s_Instance;
	};



	class RGF_API ShaderManager {


		public :
			ShaderManager() {}

			struct ShaderIndexData {
				std::string Name;
				RGF::Shader* Shader;

			};

			~ShaderManager() {
				for (unsigned int i = 0; i < m_ShaderIndex.size(); i++) {
					delete m_ShaderIndex[i].Shader;
				}

			}

			void Add(Shader* shader, const std::string& name) {
				m_ShaderIndex.push_back({ name, shader });
;			}


			void Delete(const std::string& name) {
				for (unsigned int i = 0; i < m_ShaderIndex.size(); i++) {
					if (m_ShaderIndex[i].Name == name) {
						delete m_ShaderIndex[i].Shader;
						m_ShaderIndex.erase(m_ShaderIndex.begin() + i);
						break;
					}
				}

			}

			void Delete(unsigned int index) {
				delete m_ShaderIndex[index].Shader;
				m_ShaderIndex.erase(m_ShaderIndex.begin() + index);
			}


			void Delete(RGF::Shader* shader) {
				for (unsigned int i = 0; i < m_ShaderIndex.size(); i++) {
					if (m_ShaderIndex[i].Shader == shader) {
						delete m_ShaderIndex[i].Shader;
						m_ShaderIndex.erase(m_ShaderIndex.begin() + i);
						break;
					}
				}

			}


			RGF::Shader* Find(RGF::Shader* shader) {
				for (unsigned int i = 0; i < m_ShaderIndex.size(); i++) {
					if (shader == m_ShaderIndex[i].Shader)
						return m_ShaderIndex[i].Shader;
				}
			}


			std::string& Find(const std::string& name) {
				for (unsigned int i = 0; i < m_ShaderIndex.size(); i++) {
					if (name == m_ShaderIndex[i].Name)
						return m_ShaderIndex[i].Name;
				}
			}
			std::vector<ShaderIndexData>& GetShaderIndex() { return m_ShaderIndex; }


		private :
			std::vector<ShaderIndexData> m_ShaderIndex;
	};
}