#pragma once



// This class may not be needed if im going to just use openal?
// But in the future i may want to experiment with other API's
// So this is going to be included for now.

namespace RGF {

	class AudioContext {
		public :

			enum class ContextAPI {
				None = -1,
				OPENAL
			};


			virtual void Init() = 0;
			virtual void Close() = 0;

			static ContextAPI GetContext() { return m_Context; }
			static AudioContext* CreateContext(const ContextAPI& context);

		protected :
			static ContextAPI m_Context;
	};

}