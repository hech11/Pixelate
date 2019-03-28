#pragma once

#include "Renderer.h"
#include "RetroGF/Core.h"


namespace RGF {

	enum class RGF_API RenderingType {
		None = -1,
		Default,
		Batch
	};
	class RGF_API Renderer2D : public Renderer {		

		public :
		

			static Renderer2D* Create(const RenderingType& type = RenderingType::Batch);

	};

}