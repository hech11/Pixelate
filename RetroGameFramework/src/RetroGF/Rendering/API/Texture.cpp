#include "Texture.h"
#include "RGFpch.h"


#include "RetroGF/Rendering/RenderingContext.h"
#include "RetroGF/Platform/OpenGL/GLTexture.h"


namespace RGF {

	Ref<Texture> Texture::Create(const std::string& filepath, TextureProperties props) {
		switch (RenderingContext::GetContext()) {
			case RenderingContext::ContextAPI::OPENGL:
				return CreateRef<GLTexture>(filepath, props);
			}
	}
	Ref<Texture> Texture::Create(unsigned int width, unsigned int height, Texture::TextureProperties::Format format) {
		switch (RenderingContext::GetContext()) {
			case RenderingContext::ContextAPI::OPENGL:
				return CreateRef<GLTexture>(width, height, format);
			}
	}
}