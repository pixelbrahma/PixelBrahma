#pragma once

#include "Event.h"

#include "PixelBrahma/Input/KeyCodes.h"

namespace PixelBrahma
{
	// Class key event
	class KeyEvent : public Event
	{
	public:

		// Keycode getter function
		KeyCode GetKeyCode() const { return m_KeyCode; }

		// Event class descriptions
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(const KeyCode keycode) : m_KeyCode(keycode) {}

		KeyCode m_KeyCode;
	};

	// Key pressed event class
	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const KeyCode keycode, bool isRepeat = false) : KeyEvent(keycode), 
			m_IsRepeat(isRepeat) {}

		// Repeat count getter function
		bool IsRepeat() { return m_IsRepeat; }

		// Override to string function for logging
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (repeat = " << m_IsRepeat << ")";
			return ss.str();
		}

		// Event class descriptions
		EVENT_CLASS_TYPE(KeyPressed)

	private:
		bool m_IsRepeat;
	};

	// Key released event class
	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(const KeyCode keycode) : KeyEvent(keycode) {}

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

	// Key typed event class
	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(const KeyCode keycode) : KeyEvent(keycode) {}

		// Override to string function for logging
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		// Event class descriptions
		EVENT_CLASS_TYPE(KeyTyped)
	};
}
