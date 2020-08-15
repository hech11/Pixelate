#pragma once

/* 
	Event System.
	Contains all the events that PX will interrupt and dispatch when an event is called.

	Currently contains key, mouse and window events.
*/

#include "Pixelate/Core/Core.h"
#include "PXpch.h"



/*
	TODO: Implement "EventCategories" so PX knows what type of event it is dispatching.
		  All events could be masked via a bitfield and then polled.
*/

/* 
   TODO: This event system is currently blocking all events. 
		 In the future i may want to implement a queued event system instead of blocking all events.
*/



namespace Pixelate {



	// All the events that PX supports.
	enum class EventType {
		None = 0,
		KeyPressed, KeyReleased,
		MouseMoved, MouseButtonPressed, MouseButtonReleased, MouseScrolled,
		WindowClose, WindowResize, WindowLosedFocus, WindowGainedFocus, WindowMoved
	};

	/*
		This macro is created because adding all overrides for the virtual functions in the "Event" class are very trivial and teadious...
		So this macro is created to just override all the virutal functions quickly under one alias.
		This is to be used in the classes that inherent the "Event" base class.
	*/
	#define EVENT_CLASS_TYPE(x) static EventType GetStaticEventType() {return EventType::##x;}\
								virtual EventType GetEventType() const override{return GetStaticEventType();}\
								virtual const char* GetName() const override{ return #x;}\


	enum EventCategory {
		None = 0,
		eWindow			=	BIT(0),
		eInput			=	BIT(1),
		eKeyboard		=	BIT(2),
		eMouse			=	BIT(3),
		eMouseButton	=	BIT(4)
	};

	#define EVENT_CATEGORY_TYPE(category) virtual int GetCategoryFlags() const override { return category;}



	// Event base class -- just a interface.
	class PX_API Event {
		public :
			bool Handled = false;

			virtual EventType GetEventType() const = 0;
			virtual const char* GetName()const  = 0;
			virtual std::string ToString() const { return GetName(); };

			virtual int GetCategoryFlags() const = 0;

			bool IsInCategory(EventCategory category) {
				return GetCategoryFlags() & category;
			}

	};


	/*
		The dispatcher will receive an event and dispatch it appropriately to the correct event type
		via a callback ( which is just a function pointer. Then that is executed depending if the event was handled or not. ) 
	*/

	class EventDispatcher {

		template<typename T>
		using EventFnc = std::function<bool(T&)>; // the function pointer is made under a alias

		public :
			EventDispatcher(Event& event) : m_Event(event) {}


			template<typename T>
			bool Dispatch(EventFnc<T> func) { // the function pointer alias is used here

				if (m_Event.GetEventType() == T::GetStaticEventType()) {
					m_Event.Handled = func(*(T*)&m_Event);
					return true;
				}


				return false;
			}

		private :
			Event& m_Event;
	};


}