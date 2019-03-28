#pragma once

#include "RetroGF/Rendering/Renderable.h"
#include "RetroGF/Rendering/Renderer2D.h"

#include "RGFpch.h"

namespace RGF {
	class RGF_API GLRenderer2D : public Renderer2D {

		public :

			void Clear() override;

			void Submit(const Renderable* renderable) override;
			void Render() override;

			void Start() override{}
			void End() override{}


			void Init() override{}
			void ShutDown() override{}

		private :
			std::deque<const Renderable*> m_RenderQueue; // This is temp. need to implement a proper render queue class prehaps?
	};
}