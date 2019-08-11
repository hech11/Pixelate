#pragma once

#include "RGFpch.h"
#include "RetroGF/Rendering/API/Shader/Shader.h"
#include "RetroGF/Rendering/API/Shader/ShaderUniforms.h"


// TODO: This needs to be redeigned!  : Materials and shaderuniforms!

namespace RGF {

	class Material {

		public:	
	
			Material(RGF::Shader* shader, const std::string name) : m_Shader(shader), m_Name(name)
			
			{
				
			}

			void AddUniforms(ShaderUniform* uniform) {
				uniform->Location = m_Shader->GetUniformLocation(uniform->Name);
				m_Uniforms.push_back(uniform);
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
			Shader* m_Shader = nullptr;
			std::string m_Name;
			std::vector<ShaderUniform*> m_Uniforms;


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
						mat->GetShader()->SetUniform1i(mat->GetUniforms()[0]->Name, *mat->GetUniforms()[0]->GetIntData());
				}
			}


		private :
			std::vector<Material> m_Materials;
	};

}