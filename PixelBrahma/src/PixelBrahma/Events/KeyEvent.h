#pragma once

#include "Event.h"

namespace PixelBrahma
{
	// Class key event
	class PIXELBRAHMA_API KeyEvent : public Event
	{
	public:

		// Keycode getter function
		inline int GetKeyCode() const { return m_KeyCode; }

		// Event class descriptions
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(int keycode) : m_KeyCode(keycode) {}

		int m_KeyCode;
	};

	// Key pressed event class
	class PIXELBRAHMA_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatcount) : KeyEvent(keycode), m_RepeatCount(repeatcount) {}

		// Repeat count getter function
		inline int GetRepeatCount() { return m_RepeatCount; }

		// Override to string function for logging
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		// Event class descriptions
		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int m_RepeatCount;
	};

	// Key released event class
	class PIXELBRAHMA_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

		// Override to string function for logging
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		// Event class descriptions
		EVENT_CLASS_TYPE(KeyReleased)
	};
}