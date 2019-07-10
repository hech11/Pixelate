#pragma once

#include "RGFpch.h"

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


			virtual void Init(void* window) = 0; // TODO : When OS specific headers are used, maybe rethink this design.
			virtual void SwapBuffers() = 0;

			virtual std::string GetContextName() const = 0;
			virtual std::string GetVendorName() const = 0;
			virtual std::string GetVersion() const = 0;
			virtual std::string GetRendererName() const = 0;

			static ContextAPI GetContext() { return m_Context; }

			static RenderingContext* CreateContext(const ContextAPI& context);
			
		protected:
			static ContextAPI m_Context;

	};

}