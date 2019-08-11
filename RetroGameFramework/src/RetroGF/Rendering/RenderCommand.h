#pragma once


#include "RendererAPI.h"
#include "RetroGF\Core.h"


namespace RGF {

	class RGF_API RenderCommand {

		public :

			inline static void Clear() { s_API->Clear(); };

			inline static void SetClearColor(float r, float g, float b, float a) {
				s_API->SetClearColor(r, g, b, a);
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

			inline static void SetBlending(bool enable) {
				s_API->SetBlending(enable);
			}
			inline static void SetBlendFunc(RendererAPI::BlendFunc& source, RendererAPI::BlendFunc& dest) {
				s_API->SetBlendFunc(source, dest);
			}
			
			inline static void* MapBuffer(bool enable) { return s_API->MapBuffer(enable); }


			inline static void DrawElements(VertexArray* vao) { return s_API->DrawElements(vao); }


		private :
			static RendererAPI* s_API;
	};

}