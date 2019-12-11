#pragma once

#include "Shader.h"

namespace RGF {

	class RGF_API ShaderManager {


		public:
			ShaderManager() {}
	
			struct ShaderIndexData {
				std::string Name;
				Ref<RGF::Shader> Shader;
	
			};
	
			~ShaderManager() {

			}
	
			void Add(const Ref<Shader>& shader, const std::string& name) {
				m_ShaderIndex.push_back({ name, shader });
			}
	
	
			void Delete(const std::string& name) {
				for (unsigned int i = 0; i < m_ShaderIndex.size(); i++) {
					if (m_ShaderIndex[i].Name == name) {
						m_ShaderIndex.erase(m_ShaderIndex.begin() + i);
						break;
					}
				}
	
			}
	
			void Delete(unsigned int index) {
				m_ShaderIndex.erase(m_ShaderIndex.begin() + index);
			}
	
	
			void Delete(const Ref<RGF::Shader>& shader) {
				for (unsigned int i = 0; i < m_ShaderIndex.size(); i++) {
					if (m_ShaderIndex[i].Shader == shader) {
						m_ShaderIndex.erase(m_ShaderIndex.begin() + i);
						break;
					}
				}
	
			}
	
	
			ShaderIndexData& Find(const Ref<RGF::Shader>& shader) {
				for (auto & i : m_ShaderIndex) {
					if (shader == i.Shader)
						return i;
				}
			}
	
	
			ShaderIndexData& Find(const std::string& name) {
				for (auto & i : m_ShaderIndex) {
					if (name == i.Name)
						return i;
				}
			}



			std::vector<ShaderIndexData>& GetShaderIndex() { return m_ShaderIndex; }
	
	
		private:
			std::vector<ShaderIndexData> m_ShaderIndex;
		};
}