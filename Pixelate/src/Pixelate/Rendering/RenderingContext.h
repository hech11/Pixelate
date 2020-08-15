#pragma once

#include "PXpch.h"

// Rendering Context interface.

// This decides what graphics API that PX will use.
// The 'CreateContext' method will decide depending on the API choice. OpenGL, Directx 11 or 12, vulkan etc.

namespace Pixelate {

	class RenderingContext {
		public :

			enum class ContextAPI {
				None = -1,
				OPENGL,
			};


			virtual void Init(void* window) = 0; // TODO : When OS specific headers are used, maybe rethink this design.
			virtual void SwapBuffers() = 0;

			virtual ContextAPI GetContext() const = 0;

			static RenderingContext* CreateContext(const ContextAPI& context);

	};

}