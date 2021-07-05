#pragma once

#include "Pixelate/Core/Log.h"
#include "Pixelate/Editor/EditorPanel.h"


namespace Pixelate {



	class EditorConsoleLogger : public EditorPanel {

		public :

			EditorConsoleLogger(bool registerCallbacks);
			
			void OnImguiRender() override;
		private :
			LoggingCallbacks m_Callbacks;
			bool m_AutoScroll = true;
	};


}