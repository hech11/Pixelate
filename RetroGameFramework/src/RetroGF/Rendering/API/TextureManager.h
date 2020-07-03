#pragma once

//Temp. An asset manager will refactor and replace this.


#include "RGFpch.h"
#include "RetroGF/Core/Core.h"

#include "RetroGF/Rendering/API/Texture.h"
namespace RGF {

	class TextureManager {
		public :
			struct TexturePairData {
				std::string Name;
				Ref<RGF::Texture> Texture;
			};


			TextureManager(){}
			~TextureManager(){}

			void Add(const std::string& name, const Ref<Texture>& texture) {
				m_Textures.push_back({ name, texture });
			}

	
			void Delete(const std::string& name) {
				for (unsigned int i = 0; i < m_Textures.size(); i++) {
					if (m_Textures[i].Name == name) {
						m_Textures.erase(m_Textures.begin() + i);
						break;
					}
				}
	
			}
	
			void Delete(unsigned int index) {
				m_Textures.erase(m_Textures.begin() + index);
			}
	
	
			void Delete(const Ref<RGF::Texture>& texture) {
				for (unsigned int i = 0; i < m_Textures.size(); i++) {
					if (m_Textures[i].Texture == texture) {
						m_Textures.erase(m_Textures.begin() + i);
						break;
					}
				}
	
			}
	

	
			TexturePairData& Find(const std::string& name) {
				for (auto & Texture : m_Textures) {
					if (name == Texture.Name)
						return Texture;
				}
			}

			TexturePairData& Find(const Ref<Texture>& texture) {
				for (auto & Texture : m_Textures) {
					if (texture == Texture.Texture)
						return Texture;
				}
			}

			Ref<Texture>& GetCurrentlyBound() {
				for (auto& texture : m_Textures) {
					if (texture.Texture->IsBound())
						return texture.Texture;
				}
			}

			std::vector<TexturePairData>& GetTextures() { return m_Textures; }


		private :
			std::vector<TexturePairData> m_Textures;
	};

}