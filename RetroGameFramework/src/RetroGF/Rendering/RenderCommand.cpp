#include "RGFpch.h"
#include "RenderCommand.h"



#include "RetroGF\Platform\OpenGL\GLRendererAPI.h"

namespace RGF {
	RendererAPI* RenderCommand::s_API = new GLRendererAPI;
	
}