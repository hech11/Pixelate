#pragma once

#include "RetroGF/Core/Core.h"
#include "API\VertexArray.h"

namespace RGF {


	struct RenderAPICapabilities {

		std::string ContextName, VendorName, Version, RendererName;
		int MaxTextureSlots;
	};

	class RendererAPI {

		public :
			enum class API {
				None = -1, OpenGL
			};



			enum class RGF_API BlendFunc {
				None = -1,
				ZERO,
				ONE,
				SRC_COLOR,
				ONE_MINUS_SRC_COLOR,
				DST_COLOR,
				ONE_MINUS_DST_COLOR,
				SRC_ALPHA,
				ONE_MINUS_SRC_ALPHA,
				DST_ALPHA,
				ONE_MINUS_DST_ALPHA,
				CONSTANT_COLOR,
				ONE_MINUS_CONSTANT_COLOR,
				CONSTANT_ALPHA,
				ONE_MINUS_CONSTANT_ALPHA
			};


			virtual void Init() = 0;

			virtual void Clear() = 0;
			virtual void SetClearColor(float r, float g, float b, float a) = 0;

			virtual void SetViewport(int x, int y, int width, int height) = 0;
			virtual void SetDepthTesting(bool enable) = 0;
			virtual void SetStencilTesting(bool enable) = 0;
			virtual void RenderWireFrame(bool enable) = 0;

			virtual void SetBlending(bool enable) = 0;
			virtual void SetBlendFunc(BlendFunc& source, BlendFunc& dest) = 0;


			virtual void DrawElements(const Ref<VertexArray>& vao, unsigned int count) = 0;

			virtual const RenderAPICapabilities& GetCaps() const = 0;

			inline static const API& GetAPI(){ return m_API; }

		protected :
			static API m_API;


	};

}