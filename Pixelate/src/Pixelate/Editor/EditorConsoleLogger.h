#pragma once

#include "Pixelate/Core/Log.h"


namespace Pixelate {



	class EditorConsoleLogger {

		public :

			EditorConsoleLogger(bool registerCallbacks);
			
			void OnImguiRender();
		private :
			LoggingCallbacks m_Callbacks;

			bool m_AutoScroll = true;
	};


}