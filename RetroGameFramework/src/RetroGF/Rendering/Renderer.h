#pragma once

#include "Renderable.h"


namespace RGF {

	class Renderer {

		public :
			virtual void SetDepthTesting(bool enable) = 0;
			virtual void SetStencilTesting(bool enable) = 0;
			virtual void RenderWireFrame(bool enable) = 0;


			virtual void ClearColor(float r, float g, float b) = 0;
			virtual void ClearColor(unsigned char r, unsigned char g, unsigned char b) = 0;

			virtual void Clear() = 0;

			virtual void Submit(const Renderable* renderable) = 0;
			virtual void Render() = 0;

			virtual void Start() = 0;
			virtual void End() = 0;


			virtual void Init() = 0;
			virtual void ShutDown() = 0;

	};

}