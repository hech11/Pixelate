#include "Texture.h"
#include "PXpch.h"


#include "Pixelate/Rendering/RendererAPI.h"
#include "Pixelate/Platform/OpenGL/GLTexture.h"


namespace Pixelate {

	Ref<Texture> Texture::Create(const std::string& filepath, TextureProperties props) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return CreateRef<GLTexture>(filepath, props);
			}
	}
	Ref<Texture> Texture::Create(unsigned int width, unsigned int height, TextureProperties::Format format) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return CreateRef<GLTexture>(width, height, format);
			}
	}
}