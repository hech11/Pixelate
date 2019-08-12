#include "Texture.h"
#include "RGFpch.h"


#include "RetroGF/Rendering/RenderingContext.h"
#include "RetroGF/Platform/OpenGL/GLTexture.h"


namespace RGF {
	Ref<Texture> Texture::Create(unsigned int width, unsigned int height, TextureParameters params) {
		switch (RenderingContext::GetContext()) {
			case RenderingContext::ContextAPI::OPENGL:
				return std::make_shared<GLTexture>(width, height, params);
		}
	}

}