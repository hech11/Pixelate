#pragma once



#include "Pixelate/Core/Core.h"
#include "PXpch.h"

#include "Pixelate/Events/Event.h"

/*
	All mouse events that PX uses:
	MouseMovedEvent, MouseButtonPressedEvent, MouseButtonReleasedEvent
	and MouseScrolledEvent.
*/


/*
	 The mouse events are derived from the "event" base class and implements
	 all their virtual functions via "EVENT_CLASS_TYPE" ( see "Event.h" for more info ).
*/

namespace Pixelate {

	// All the mouse events :

	class PX_API MouseMovedEvent : public Event {
		public :
			MouseMovedEvent(int xPos, int yPos) : 
				m_xPos(xPos), m_yPos(yPos) {}

			std::string ToString() const override {
				std::stringstream ss;
				ss << "MouseMovedEvent (:" << m_xPos << ", " << m_yPos << ")";
				return ss.str();
			};

			EVENT_CLASS_TYPE(MouseMoved);
			EVENT_CATEGORY_TYPE(EventCategory::eInput | EventCategory::eMouse)


			std::pair<int, int> GetMousePos() const { return std::make_pair(m_xPos, m_yPos); }
		private :
			int m_xPos, m_yPos;
	};
	


	class PX_API MouseButtonPressedEvent : public Event {
		public :
			MouseButtonPressedEvent(int button, int repeatCount) :
				m_Button(button), m_RepeatCount(repeatCount) {}

			std::string ToString() const override {
				std::stringstream ss;
				ss << "MouseButtonPressedEvent : Button : " << m_Button << ", RepeatCount : " << m_RepeatCount;
				return ss.str();
			};

			int GetButton() const { return m_Button; }
			int GetRepeatCount() const { return m_RepeatCount; }

			EVENT_CLASS_TYPE(MouseButtonPressed);
			EVENT_CATEGORY_TYPE(EventCategory::eInput | EventCategory::eMouseButton)

		private :
			int m_Button, m_RepeatCount;
	};




	class PX_API MouseButtonReleasedEvent : public Event {
		public :
			MouseButtonReleasedEvent(int button) :
				m_Button(button) {}

			std::string ToString() const override {
				std::stringstream ss;
				ss << "MouseButtonReleasedEvent : Button : " << m_Button;
				return ss.str();
			};

			int GetButton() const { return m_Button; }

			EVENT_CLASS_TYPE(MouseButtonReleased);
			EVENT_CATEGORY_TYPE(EventCategory::eInput | EventCategory::eMouseButton)

		private :
			int m_Button;
	};



	class PX_API MouseScrolledEvent : public Event {
		public :
			MouseScrolledEvent(int xScroll, int yScroll) :
				m_xScroll(xScroll), m_yScroll(yScroll) {}

			std::string ToString() const override {
				std::stringstream ss;
				ss << "MouseScrolledEvent : (" << m_xScroll << ", " << m_yScroll << ")";
				return ss.str();
			};
			
			EVENT_CLASS_TYPE(MouseScrolled);
			EVENT_CATEGORY_TYPE(EventCategory::eInput | EventCategory::eMouse)

			int GetXScroll() const { return m_xScroll; }
			int GetYScroll() const { return m_yScroll; }


		private :
			int m_xScroll, m_yScroll;
	};

}