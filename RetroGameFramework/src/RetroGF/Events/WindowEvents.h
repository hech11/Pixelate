#pragma once


#include "RetroGF/Core/Core.h"
#include "RGFpch.h"

#include "RetroGF/Events/Event.h"



/* 
	All window events that RGF uses :
	WindowCloseEvent, WindowResizeEvent, WindowLosedFocusEvent, 
	WindowGainedFocusEvent and WindowMovedEvent.
*/

/*
	 The window events are derived from the "event" base class and implements
	 all their virtual functions via "EVENT_CLASS_TYPE" ( see "Event.h" for more info ).
*/


namespace RGF {


	// All the window Events:

	class RGF_API WindowCloseEvent : public Event {
		public : 
			WindowCloseEvent() {}
			

			EVENT_CLASS_TYPE(WindowClose);
			EVENT_CATEGORY_TYPE(EventCategory::eWindow)

	};


	class RGF_API WindowResizeEvent: public Event {
		public : 
			WindowResizeEvent(unsigned int width, unsigned int height) 
				: m_Width(width), m_Height(height) {}



			inline unsigned int GetWidth() const { return m_Width; }
			inline unsigned int GetHeight() const { return m_Height; }


			std::string ToString() const override { 
				std::stringstream ss;
				ss << "WindowResizeEvent: (" << m_Width << ", " << m_Height << ")";
				return ss.str();
			};

			EVENT_CLASS_TYPE(WindowResize);
			EVENT_CATEGORY_TYPE(EventCategory::eWindow)


		private :
			unsigned int m_Width, m_Height;
	};


	class RGF_API WindowLosedFocusEvent : public Event {
		public:
			WindowLosedFocusEvent() {}
	
			EVENT_CLASS_TYPE(WindowLosedFocus);
			EVENT_CATEGORY_TYPE(EventCategory::eWindow)
	};

	class RGF_API WindowGainedFocusEvent : public Event {
		public:
			WindowGainedFocusEvent() {}
			
			EVENT_CLASS_TYPE(WindowGainedFocus);
			EVENT_CATEGORY_TYPE(EventCategory::eWindow)
	};



	class RGF_API WindowMovedEvent: public Event {
		private :
			unsigned int m_WinPosX, m_WinPosY;
		public:
			WindowMovedEvent(int xpos, int ypos) : m_WinPosX(xpos), m_WinPosY(ypos) {}
			inline unsigned int GetWinPosX() const { return m_WinPosX; }
			inline unsigned int GetWinPosY() const { return m_WinPosY; }
			EVENT_CLASS_TYPE(WindowMoved);
			EVENT_CATEGORY_TYPE(EventCategory::eWindow)

	};

}