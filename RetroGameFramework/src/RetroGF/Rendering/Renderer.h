#pragma once

#include "Renderable.h"


namespace RGF {

	class Renderer {

		public :
			virtual void Clear() = 0;

			virtual void Submit(const Renderable* renderable) = 0;
			virtual void Render() = 0;

			virtual void Start() = 0;
			virtual void End() = 0;

			virtual ~Renderer() { }

			virtual void Init() = 0;
			virtual void ShutDown() = 0;
	};

}