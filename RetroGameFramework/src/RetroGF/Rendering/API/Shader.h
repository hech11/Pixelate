#pragma once

#include "RetroGF/Core.h"
#include "RGFpch.h"
#include "GLM/glm/glm.hpp"


// Shader interface.
// The 'Create' method will decide depending on the API choice. OpenGL, Directx 11 or 12, vulkan etc.

namespace RGF {

	class RGF_API Shader {

		public :
			Shader(){}
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

			~ShaderManager() {
				for (unsigned int i = 0; i < m_ShaderIndex.size(); i++) {
					delete m_ShaderIndex[i].second;
				}

			}

			void Add(Shader* shader, const std::string& name) {
				m_ShaderIndex.push_back({ name, shader });
;			}


			void Delete(const std::string& name) {
				for (unsigned int i = 0; i < m_ShaderIndex.size(); i++) {
					if (m_ShaderIndex[i].first == name) {
						m_ShaderIndex.erase(m_ShaderIndex.begin() + i);
						break;
					}
				}

			}

			void Delete(unsigned int index) {
				m_ShaderIndex.erase(m_ShaderIndex.begin() + index);
			}


			void Delete(RGF::Shader* shader) {
				for (unsigned int i = 0; i < m_ShaderIndex.size(); i++) {
					if (m_ShaderIndex[i].second == shader) {
						m_ShaderIndex.erase(m_ShaderIndex.begin() + i);
						break;
					}
				}

			}

		std::vector<std::pair<std::string, RGF::Shader*>>& GetShaderIndex() { return m_ShaderIndex; }


		private :
			std::vector<std::pair<std::string, RGF::Shader*>> m_ShaderIndex;
	};
}