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

			void SetUniforms(Ref<Material> mat) {

				mat->GetShader()->Bind();
				switch (mat->GetUniforms()[0]->Type) {
					case ShaderUnifromType::Int :
						mat->GetShader()->SetUniform1i(mat->GetUniforms()[0]->Name, *mat->GetUniforms()[0]->GetIntData());
				}
			}


		private :
			std::vector<Ref<Material>> m_Materials;
	};

}