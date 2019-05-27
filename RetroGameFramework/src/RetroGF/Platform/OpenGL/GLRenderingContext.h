#pragma once

#include "RetroGF/Rendering/RenderingContext.h"



struct GLFWwindow;
namespace RGF {


	class GLRenderingContext : public RenderingContext {
		public :
			GLRenderingContext();
			~GLRenderingContext();

			virtual void Init(void* window) override;
			virtual void SwapBuffers() override;

			virtual std::string GetContextName() const {
				return "OpengGL";
			}

			virtual std::string GetVendorName() const override;
			virtual std::string GetVersion() const override;
			virtual std::string GetRendererName() const override;



		private :
			GLFWwindow* m_WindowHandle;
	};
}