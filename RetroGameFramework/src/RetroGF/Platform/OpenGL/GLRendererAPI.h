#pragma once


#include "RetroGF\Rendering\RendererAPI.h"



namespace RGF {

	class RGF_API GLRendererAPI : public RendererAPI {

		public:

			void Init() override;

			void Clear() override;
			void SetClearColor(float r, float g, float b, float a) override;

			void SetViewport(int x, int y, int width, int height)override;


			void SetDepthTesting(bool enable) override;
			void SetStencilTesting(bool enable) override;
			void RenderWireFrame(bool enable) override;

			void SetBlending(bool enable) override;
			void SetBlendFunc(BlendFunc& source, BlendFunc& dest) override;


			void DrawElements(const Ref<VertexArray>& vao, unsigned int count) override;

			virtual const RenderAPICapabilities& GetCaps() const override { return m_Caps;}



		private:
			unsigned int ConvertBlendFunctions(const BlendFunc& func);
		private:
			RenderAPICapabilities m_Caps;

	};
}

