#include "RGFpch.h"
#include "RenderCommand.h"



#include "RetroGF\Platform\OpenGL\GLRendererAPI.h"


namespace RGF {
	RendererAPI* RenderCommand::s_API = nullptr;



	void RenderCommand::Init(const RenderingContext::ContextAPI& context) {

		switch (context) {
			case RenderingContext::ContextAPI::OPENGL:
				s_API = new GLRendererAPI;
				s_API->Init();
		}
	}

	
}