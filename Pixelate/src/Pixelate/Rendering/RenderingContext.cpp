#include "PXpch.h"
#include "RenderingContext.h"


#include "Pixelate/Platform/OpenGL/GLRenderingContext.h"

namespace Pixelate {


	RenderingContext* RenderingContext::CreateContext(const RenderingContext::ContextAPI& context) {
		switch (context) {
			case RenderingContext::ContextAPI::OPENGL:
				return new GLRenderingContext;
		}
	}

}