#pragma once

//Temp. An asset manager will refactor and replace this.


#include "PXpch.h"
#include "Pixelate/Core/Core.h"

#include "Pixelate/Rendering/API/Texture.h"
namespace Pixelate {

	class TextureManager {

		public :

			struct ManagerData {
				

				unsigned int TextureSlotIndex = 1; // 0 = white texture
				Ref<Texture> DefaultWhiteTexture;

				std::array<glm::vec2, 4> TextureCoords;
				std::vector<Ref<Texture>> AllTextures;
			};

			static void Init();
			static void Shutdown();

			static void Add(const Ref<Texture>& texture);
			static void DirectAdd(const Ref<Texture>& texture); // this adds a texture without any validation
	
			static Ref<Texture>& GetDefaultTexture();
			static Ref<Texture>& GetTexure(const Ref<Texture>& texture);
			static Ref<Texture>& GetTexture(unsigned int index);

			static ManagerData& GetManagerData();

			static unsigned int IsTextureValid(const Ref<Texture>& texture);
			static unsigned int IsTextureValid(const std::string& filepath);

			static void BindAllTextures();

	};

}