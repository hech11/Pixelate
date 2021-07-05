#include "PXpch.h"
#include "TextureManager.h"

#include "Pixelate/Rendering/RendererCapabilities.h"


namespace Pixelate {


	static TextureManager::ManagerData* s_ManagerData = new TextureManager::ManagerData;

	void TextureManager::BindAllTextures()
	{
		for (unsigned int i = 0; i < s_ManagerData->TextureSlotIndex; i++) {
			s_ManagerData->AllTextures[i]->Bind(i);
		}
	}


	unsigned int TextureManager::IsTextureValid(const Ref<Texture>& texture) {
		for (unsigned int i = 1; i < s_ManagerData->TextureSlotIndex; i++) {
			if (*s_ManagerData->AllTextures[i].get() == *texture.get()) {
				return i;
			}
		}
		return 0;
	}

	unsigned int TextureManager::IsTextureValid(const std::string& filepath) {
		for (unsigned int i = 1; i < s_ManagerData->TextureSlotIndex; i++) {
			if (s_ManagerData->AllTextures[i]->GetFilepath() == filepath) {
				return i;
			}
		}
		return 0;
	}


	void TextureManager::Init() {
		s_ManagerData->TextureCoords[0] = { 0.0f, 0.0f }; // -- bottom left
		s_ManagerData->TextureCoords[1] = { 1.0f, 0.0f }; // -- bottom right
		s_ManagerData->TextureCoords[2] = { 1.0f, 1.0f }; // -- top right
		s_ManagerData->TextureCoords[3] = { 0.0f, 1.0f }; // -- top left

		s_ManagerData->DefaultWhiteTexture = Texture::Create(1, 1, TextureProperties::Format::RGB);

		unsigned int whiteTextureData = 0xffffffff;
		s_ManagerData->DefaultWhiteTexture->SetData(&whiteTextureData, 3);

 		s_ManagerData->AllTextures.resize(RendererCapabilities::MaxTextureSlots);
 		s_ManagerData->AllTextures[0] = s_ManagerData->DefaultWhiteTexture;

	}

	void TextureManager::Shutdown()
	{
		delete s_ManagerData;
	}

	void TextureManager::Add(const Ref<Texture>& texture) {

		if (IsTextureValid(texture))
			return;

		DirectAdd(texture);

	}

	void TextureManager::DirectAdd(const Ref<Texture>& texture)
	{
		s_ManagerData->AllTextures[s_ManagerData->TextureSlotIndex] = texture;
		s_ManagerData->TextureSlotIndex++;
	}

	
	Ref<Texture>& TextureManager::GetDefaultTexture() {
		return s_ManagerData->AllTextures[0];
	}

	Ref<Texture>& TextureManager::GetTexure(const Ref<Texture>& texture) {
		if (auto id = IsTextureValid(texture)) {
			return s_ManagerData->AllTextures[id];
		}

		PX_CORE_WARN("Could not find texture '%s'\n", texture->GetFilepath().c_str());
		return GetDefaultTexture();
	}
	Ref<Texture>& TextureManager::GetTexture(unsigned int index) {
		if (IsTextureValid(s_ManagerData->AllTextures[index])) {
			return s_ManagerData->AllTextures[index];
		}

		PX_CORE_WARN("Could not find texture ID '%d'\n", index);
		return GetDefaultTexture();
	}



	Pixelate::TextureManager::ManagerData& TextureManager::GetManagerData()
	{
		return *s_ManagerData;
	}


}