#include "PXpch.h"
#include "RenderCommand.h"



#include "Pixelate\Platform\OpenGL\GLRendererAPI.h"
#include "Renderer2D.h"



namespace Pixelate {
	RendererAPI* RenderCommand::s_API = nullptr;



	void RenderCommand::Init(const RenderingContext::ContextAPI& context) {

		switch (context) {
		case RenderingContext::ContextAPI::OPENGL:
			s_API = new GLRendererAPI;
			s_API->Init();
			
		}
	}

	
}