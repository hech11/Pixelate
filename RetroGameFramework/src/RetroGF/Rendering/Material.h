#pragma once

#include "RGFpch.h"
#include "API/Shader.h"


namespace RGF {

	class Material {

		public:	
	
			Material(const std::string& name, Shader* shader) : m_Name(name), m_Shader(shader) {}
	
	
	
			void AddUniform(ShaderUniform* uniform) {
				uniform->Location = m_Shader->GetUniformLocation(uniform->Name);
				m_Uniforms.push_back(uniform);
			}
	
	
			void DeleteUniform(const std::string& name) { 
				for (unsigned int i = 0; i < m_Uniforms.size(); i++) {
					if (m_Uniforms[i]->Name == name) {
						m_Uniforms.erase(m_Uniforms.begin() + i);
						break;
					}
				}
			}
	
	
			void SetUniform4f(const std::string& uniformName, const glm::vec4& values) {
				m_Shader->Bind();
				m_Shader->SetUniform4f(uniformName, values);
			}
			void SetUniform1i(const std::string& uniformName, int value) {
				m_Shader->Bind();
				m_Shader->SetUniform1i(uniformName, value);
			}
			void SetUniformMat4(const std::string& uniformName, const glm::mat4& value) {
				m_Shader->Bind();
				m_Shader->SetUniformMatrix(uniformName, value);
			}

			void SetName(const std::string& name) { m_Name = name; }
			inline const std::string& GetName() const { return m_Name; }

			void SetShader(Shader* shader) { 
				if (m_Shader != nullptr) delete m_Shader;
				m_Shader = shader; 
			}
			inline RGF::Shader* GetShader() { return m_Shader; }


			inline std::vector<ShaderUniform*>& GetUniforms() { return m_Uniforms; }
		private:
			std::string m_Name;
			std::vector<ShaderUniform*> m_Uniforms;
			Shader* m_Shader;


	};

	class MaterialManager {
		public :

			//TODO : decide if mats should be stored on the heap
			inline std::vector<Material>& GetMaterialList() { return m_Materials; }

			void AddMaterial(const Material& mat) { m_Materials.push_back(mat); }
			void DeleteMaterial(unsigned int index) { m_Materials.erase(m_Materials.begin() + index); }

			void SetUniforms(Material* mat) {

				mat->GetShader()->Bind();
				switch (mat->GetUniforms()[0]->Type) {
					case ShaderUnifromType::Int :
						mat->SetUniform1i(mat->GetUniforms()[0]->Name, *mat->GetUniforms()[0]->GetIntData());
				}
			}


		private :
			std::vector<Material> m_Materials;
	};

}