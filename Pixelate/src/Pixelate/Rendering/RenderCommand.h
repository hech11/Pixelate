#pragma once


#include "RendererAPI.h"
#include "Pixelate/Core/Core.h"

#include "RenderingContext.h"

namespace Pixelate {

	class PX_API RenderCommand {

		public :

			static void Init(const RenderingContext::ContextAPI& context);

			inline static void Clear() { s_API->Clear(); };

			inline static void SetClearColor(float r, float g, float b, float a) {
				s_API->SetClearColor(r, g, b, a);
			}

			inline static void SetViewport(int x, int y, int width, int height) {
				s_API->SetViewport(x, y, width, height);
			}

			inline static void SetDepthTesting(bool enable) {
				s_API->SetDepthTesting(enable);
			}
			inline static void SetStencilTesting(bool enable) {
				s_API->SetStencilTesting(enable);
			}
			inline static void RenderWireFrame(bool enable) {
				s_API->RenderWireFrame(enable);
			}


			// Should this even be here if its not gonna be used?
			inline static void SetBlending(bool enable) {
				s_API->SetBlending(enable);
			}
			inline static void SetBlendFunc(RendererAPI::BlendFunc& source, RendererAPI::BlendFunc& dest) {
				s_API->SetBlendFunc(source, dest);
			}
			
			inline static void SetLineThickness(float width) { s_API->SetLineThickness(width); }

			// Not sure how i feel about this being here..
			inline static const RenderAPIData& GetAPIData() { return s_API->GetAPIData(); }

			inline static void DrawElements(const Ref<VertexArray>& vao, PimitiveRenderType type, unsigned int count) { return s_API->DrawElements(vao, type, count); }


		private :
			static RendererAPI* s_API;
	};

}