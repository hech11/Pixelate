#include "Texture.h"
#include "RGFpch.h"


#include "RetroGF/Rendering/RendererAPI.h"
#include "RetroGF/Platform/OpenGL/GLTexture.h"


namespace RGF {

	Ref<Texture> Texture::Create(const std::string& filepath, TextureProperties props) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return CreateRef<GLTexture>(filepath, props);
			}
	}
	Ref<Texture> Texture::Create(unsigned int width, unsigned int height, Texture::TextureProperties::Format format) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return CreateRef<GLTexture>(width, height, format);
			}
	}
}