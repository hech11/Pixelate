#include "RGFpch.h"
#include "Renderer2D.h"

#include "RetroGF/Platform/OpenGL/GLRenderer2D.h"


#include "RetroGF/Rendering/RenderingContext.h"

namespace RGF {

	Renderer2D* Renderer2D::Create(const RenderingType& type) {
		switch (RenderingContext::GetContext()) {
			case RenderingContext::ContextAPI::OPENGL:
				switch (type) {
					case RenderingType::Default:
						return new GLRenderer2D;
					case RenderingType::Batch:
						return new GLBatchRenderer2D;
				}
				break;

		}
		
	}
}