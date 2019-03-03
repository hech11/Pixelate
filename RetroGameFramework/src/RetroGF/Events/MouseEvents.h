#pragma once



#include "RetroGF/Core.h"
#include "RetroGF/RGFpch.h"

#include "RetroGF/Events/Event.h"

/*
	All mouse events that RGF uses:
	MouseMovedEvent, MouseButtonPressedEvent, MouseButtonReleasedEvent
	and MouseScrolledEvent.
*/


/*
	 The mouse events are derived from the "event" base class and implements
	 all their virtual functions via "EVENT_CLASS_TYPE" ( see "Event.h" for more info ).
*/

namespace RGF {

	// All the mouse events :

	class RGF_API MouseMovedEvent : public Event {
		public :
			MouseMovedEvent(int xPos, int yPos) : 
				m_xPos(xPos), m_yPos(yPos) {}

			std::string ToString() const override {
				std::stringstream ss;
				ss << "MouseMovedEvent (:" << m_xPos << ", " << m_yPos << ")";
				return ss.str();
			};

			EVENT_CLASS_TYPE(MouseMoved);

		private :
			int m_xPos, m_yPos;
	};
	


	class RGF_API MouseButtonPressedEvent : public Event {
		public :
			MouseButtonPressedEvent(int button, int repeatCount) :
				m_Button(button), m_RepeatCount(repeatCount) {}

			std::string ToString() const override {
				std::stringstream ss;
				ss << "MouseButtonPressedEvent : Button : " << m_Button << ", RepeatCount : " << m_RepeatCount;
				return ss.str();
			};

			EVENT_CLASS_TYPE(MouseButtonPressed);

		private :
			int m_Button, m_RepeatCount;
	};




	class RGF_API MouseButtonReleasedEvent : public Event {
		public :
			MouseButtonReleasedEvent(int button) :
				m_Button(button) {}

			std::string ToString() const override {
				std::stringstream ss;
				ss << "MouseButtonReleasedEvent : Button : " << m_Button;
				return ss.str();
			};
			
			EVENT_CLASS_TYPE(MouseButtonReleased);

		private :
			int m_Button;
	};



	class RGF_API MouseScrolledEvent : public Event {
		public :
			MouseScrolledEvent(int xScroll, int yScroll) :
				m_xScroll(xScroll), m_yScroll(yScroll) {}

			std::string ToString() const override {
				std::stringstream ss;
				ss << "MouseScrolledEvent : (" << m_xScroll << ", " << m_yScroll << ")";
				return ss.str();
			};
			
			EVENT_CLASS_TYPE(MouseScrolled);

		private :
			int m_xScroll, m_yScroll;
	};

}