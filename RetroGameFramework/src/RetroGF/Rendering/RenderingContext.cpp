#include "RGFpch.h"
#include "RenderingContext.h"


#include "RetroGF/Platform/OpenGL/GLRenderingContext.h"

namespace RGF {


	RenderingContext* RenderingContext::CreateContext(const RenderingContext::ContextAPI& context) {
		switch (context) {
			case RenderingContext::ContextAPI::OPENGL:
				return new GLRenderingContext;
		}
	}

}