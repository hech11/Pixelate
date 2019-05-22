#include "Texture.h"
#include "RGFpch.h"


#include "RetroGF/Rendering/RenderingContext.h"
#include "RetroGF/Platform/OpenGL/GLTexture.h"


namespace RGF {
	Texture* Texture::Create(unsigned int width, unsigned int height, TextureParameters params) {
		switch (RenderingContext::GetContext()) {
			case RenderingContext::ContextAPI::OPENGL:
				return new GLTexture(width, height, params);
		}
	}

}