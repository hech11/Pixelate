#pragma once

#include "RGFpch.h"
#include "RetroGF/Rendering/API/Shader/Shader.h"
#include "RetroGF/Rendering/API/Shader/ShaderUniforms.h"


// TODO: This needs to be redeigned!  : Materials and shaderuniforms!

namespace RGF {

	class Material {

		public:	
	
			Material(const Ref<RGF::Shader>& shader, const std::string& name) : m_Shader(shader), m_Name(name)
			
			{
				
			}

			~Material() 
			{
				WeakRef<Shader> s = m_Shader;
				RGF_ASSERT(!s.expired(), "Shader has been deleted!");
			}

			void AddUniforms(ShaderUniform* uniform) {
				Ref<ShaderUniform> Uni;
				Uni.reset(uniform);

				Uni->Location = m_Shader->GetUniformLocation(Uni->Name);
				m_Uniforms.push_back(Uni);
			}
	
	
			void SetName(const std::string& name) { m_Name = name; }
			inline const std::string& GetName() const { return m_Name; }
			void SetShader(const Ref<Shader>& shader) {
				if (m_Shader != nullptr) m_Shader.reset();
				m_Shader = shader; 
			}
			inline Ref<Shader>& GetShader() { return m_Shader; }


			inline std::vector<Ref<ShaderUniform>>& GetUniforms() { return m_Uniforms; }
		private:
			Ref<Shader> m_Shader;
			std::string m_Name;
			std::vector<Ref<ShaderUniform>> m_Uniforms;


	};

	class MaterialManager {
		public :

			//TODO : decide if mats should be stored on the heap
			inline std::vector<Ref<Material>>& GetMaterialList() { return m_Materials; }

			void AddMaterial(const Ref<Material>& mat) { m_Materials.push_back(mat); }
			void DeleteMaterial(unsigned int index) { m_Materials.erase(m_Materials.begin() + index); }




			void SumbitUniforms(const Ref<Material>& mat, const Ref<ShaderUniform>& uniform) 
			{
				switch (uniform->Type) {
					case ShaderUnifromType::Int :
						mat->GetShader()->SetUniform1i(uniform->Name, *(int*)uniform->ReturnData());
						break;


					case ShaderUnifromType::Int2:
						mat->GetShader()->SetUniform2i(uniform->Name, *(glm::i32vec2*)uniform->ReturnData());
						break;


					case ShaderUnifromType::Int3:
						mat->GetShader()->SetUniform3i(uniform->Name, *(glm::i32vec3*)uniform->ReturnData());
						break;



					case ShaderUnifromType::Int4:
						mat->GetShader()->SetUniform4i(uniform->Name, *(glm::i32vec4*)uniform->ReturnData());
						break;



					case ShaderUnifromType::Char:
						mat->GetShader()->SetUniform1i(uniform->Name, *(char*)uniform->ReturnData());
						break;



					case ShaderUnifromType::Char2:
						mat->GetShader()->SetUniform2i(uniform->Name, *(glm::i8vec2*)uniform->ReturnData());
						break;




					case ShaderUnifromType::Char3:
						mat->GetShader()->SetUniform3i(uniform->Name, *(glm::i8vec3*)uniform->ReturnData());
						break;




					case ShaderUnifromType::Char4:
						mat->GetShader()->SetUniform4i(uniform->Name, *(glm::i8vec4*)uniform->ReturnData());
						break;



					case ShaderUnifromType::Float:
						mat->GetShader()->SetUniform1f(uniform->Name, *(float*)uniform->ReturnData());
						break;

					case ShaderUnifromType::Float2:
						mat->GetShader()->SetUniform2f(uniform->Name, *(glm::vec2*)uniform->ReturnData());
						break;



					case ShaderUnifromType::Float3:
						mat->GetShader()->SetUniform3f(uniform->Name, *(glm::vec3*)uniform->ReturnData());
						break;



					case ShaderUnifromType::Float4:
						mat->GetShader()->SetUniform4f(uniform->Name, *(glm::vec4*)uniform->ReturnData());
						break;


					case ShaderUnifromType::Mat4:
						mat->GetShader()->SetUniformMatrix(uniform->Name, *(glm::mat4*)uniform->ReturnData());
						break;
				}
			}


		private :
			std::vector<Ref<Material>> m_Materials;
	};

}