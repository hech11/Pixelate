#pragma once

#include "RGFpch.h"

// TODO: is "Rendering Context" a good name? Maybe rename to just "Context"

// Rendering Context interface.

// This decides what graphics API that RGF will use.
// The 'CreateContext' method will decide depending on the API choice. OpenGL, Directx 11 or 12, vulkan etc.

namespace RGF {

	class RenderingContext {
		public :
			enum class ContextAPI {
				None = -1,
				OPENGL,
				DIRECT11
			};


			virtual void Init(void* window) = 0;
			virtual void SwapBuffers() = 0;

			virtual std::string GetContextName() const = 0;
			virtual std::string GetVendorName() const = 0;
			virtual std::string GetVersion() const = 0;
			virtual std::string GetRendererName() const = 0;


			static RenderingContext* CreateContext(const ContextAPI& context);
			static ContextAPI GetContext() { return m_Context; }
			
		protected:
			static ContextAPI m_Context;

	};

}