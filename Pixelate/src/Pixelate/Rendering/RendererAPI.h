#pragma once

#include "Pixelate/Core/Core.h"
#include "API\VertexArray.h"

namespace Pixelate {


	struct RenderAPIData {

		std::string ContextName, VendorName, Version, RendererName;
	};

	enum class PimitiveRenderType {
		None = 0,
		Triangles,
		Lines
	};

	class RendererAPI {

		public :
			enum class API {
				None = -1, OpenGL
			};



			enum class PX_API BlendFunc {
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
			virtual void SetLineThickness(float width) = 0;

			virtual void DrawElements(const Ref<VertexArray>& vao, PimitiveRenderType type, unsigned int count) = 0;

			virtual const RenderAPIData& GetAPIData() const = 0;

			inline static const API& GetAPI(){ return m_API; }

		protected :
			static API m_API;


	};

}