#pragma once

#include "PixelBrahma/Debug/Instrumentor.h"
#include "PixelBrahma/Core/Core.h"

#include <functional>

// Events are blocking events
// Immediately dispatched and must be dealt with immediately
// Could be updated to a buffered event system

namespace PixelBrahma
{
	// Enum class of event types
	enum class EventType
	{
		None = 0,

		// Window events
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,

		// Application events
		AppTick, AppUpdate, AppRender,

		// Key events
		KeyPressed, KeyReleased, KeyTyped,

		// Mouse events
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	// Enum of event categories - To attach a trait to event types
	// Bitfields are used as event categories are not exclusive
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication      = BIT(0),
		EventCategoryInput            = BIT(1),
		EventCategoryKeyboard         = BIT(2),
		EventCategoryMouse            = BIT(3),
		EventCategoryMouseButton      = BIT(4)
	};

// Event class type macro
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
                  virtual EventType GetEventType() const override { return GetStaticType(); }\
                  virtual const char* GetName() const override { return #type; }

// Event class category macro
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	// Event class
	class Event
	{
		friend class EventDispatcher;

	public:
		virtual ~Event() = default;

		bool Handled = false;

		// Virtual event properties getter functions

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;

		// Virtual to string function for logging
		virtual std::string ToString() const { return GetName(); }

		// Check if an event is in the given category
		bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	};

	// Event dispatcher class
	class EventDispatcher
	{
		template<typename T>

		// Function that returns bool and takes in an event argument
		using EventFn = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& event) : m_Event(event) {}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			// If event type matched the handler function type
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				// Handle the event by calling the function
				m_Event.Handled |= func(static_cast<T&>(m_Event));
				return true;
			}

			return false;
		}

	private:
		Event& m_Event;
	};

	// Operator overloading for logging
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}
