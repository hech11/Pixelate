#include "RGFpch.h"
#include "Renderer2D.h"

#include "RetroGF/Platform/OpenGL/GLBatchRenderer2D.h"
#include "RetroGF/Platform/OpenGL/GLRenderer2D.h"

namespace RGF {

	Renderer2D* Renderer2D::Create(const RenderingType& type) {
		//TODO: This is temp. When/if other grahpical API's are added please change this!
		switch (type) {
		case RenderingType::Default:
			return new GLRenderer2D;
		case RenderingType::Batch:
			return new GLBatchRenderer2D;
		}
	}
}