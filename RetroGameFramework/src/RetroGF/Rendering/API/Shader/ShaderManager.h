#pragma once

#include "Shader.h"

namespace RGF {

	class RGF_API ShaderManager {


		public:
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
			}
	
	
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
	
	
		private:
			std::vector<ShaderIndexData> m_ShaderIndex;
		};
}