#pragma once



// This class may not be needed if im going to just use openal?
// But in the future i may want to experiment with other API's
// So this is going to be included for now.

namespace Pixelate {

	class AudioContext {
		public :

			enum class ContextAPI {
				None = -1,
				OpenAL
			};

			void Init();
			void Close();

			ContextAPI GetContext() const { return m_Context; }

		private :
			ContextAPI m_Context;
	};

}