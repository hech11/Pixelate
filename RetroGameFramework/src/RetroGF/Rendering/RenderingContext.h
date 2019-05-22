#pragma once

#include "RGFpch.h"

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

			static RenderingContext* CreateContext(const ContextAPI& context);
			static ContextAPI GetContext() { return m_Context; }
			
		protected:
			static ContextAPI m_Context;

	};

}