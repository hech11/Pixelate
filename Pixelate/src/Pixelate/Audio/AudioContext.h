#pragma once


namespace Pixelate {

	class AudioContext {
		public :

			enum class ContextAPI {
				None = -1,
				OpenAL
			};

			bool Init(); // return true if initialized else returns false
			void Close();

			ContextAPI GetContext() const { return m_Context; }

		private :
			ContextAPI m_Context;
	};

}