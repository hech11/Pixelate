#pragma once


#include "Renderer.h"
#include "RetroGF/Core.h"


namespace RGF {

	
	class RGF_API Renderer2D : public Renderer {		

		public :
			enum class RGF_API RenderingType {
				None = -1,
				Default,
				Batch
			};

			Renderer2D(const RenderingType& type) : m_Type(type) {}
			static Renderer2D* Create(const RenderingType& type = RenderingType::Batch);

		protected :
			RenderingType m_Type = RenderingType::None;

	};

}