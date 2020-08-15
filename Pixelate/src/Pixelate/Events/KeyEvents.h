#pragma once

#include "Pixelate/Core/Core.h"
#include "PXpch.h"

#include "Pixelate/Events/Event.h"



/* 
	All key events that PX uses :
	KeyPressedEvent, KeyReleasedEvent.
*/

/*
	 The key events are derived from the "event" base class and implements
	 all their virtual functions via "EVENT_CLASS_TYPE" ( see "Event.h" for more info ).
 */


namespace Pixelate {


	/*
		This "KeyEvent" class is made because all of the key events contain a "keycode".
		Each key event will inherent from the "KeyEvent" class
	*/
	class PX_API KeyEvent : public Event { 
		public :
			inline int GetKeyCode() const { return m_Keycode; }
			EVENT_CATEGORY_TYPE(EventCategory::eInput | EventCategory::eKeyboard)
		protected :
			KeyEvent(int keycode) : m_Keycode(keycode) {}
			int m_Keycode;
	};



	// All the key events:

	class PX_API KeyPressedEvent : public KeyEvent {
		public :
			KeyPressedEvent(int keycode, int repeatCount) : KeyEvent(keycode), m_RepeatCount(repeatCount){}

			inline int GetRepeatCount() const { return m_RepeatCount; }


			std::string ToString() const override {
				std::stringstream ss;
				ss << "KeyPressedEvent: Keycode : " << m_Keycode << ", RepeatCount : " << m_RepeatCount;
				return ss.str();
			};


			EVENT_CLASS_TYPE(KeyPressed);
		private :
			unsigned int m_RepeatCount = 0;
	};


	
	class PX_API KeyReleasedEvent : public KeyEvent {
		public :
			KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}


			std::string ToString() const override {
				std::stringstream ss;
				ss << "KeyReleasedEvent: Keycode : " << m_Keycode;
				return ss.str();
			};

			EVENT_CLASS_TYPE(KeyReleased);

	};
}