#pragma once

#include "Event.h"

namespace PixelBrahma
{
	// Mouse moved event class
	class PIXELBRAHMA_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y) : m_MouseX(x), m_MouseY(y) {}

		// Mouse position getter functions

		inline float GetX() { return m_MouseX; }
		inline float GetY() { return m_MouseY; }

		// Override to string function for logging
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		// Event class descriptions
		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_MouseX, m_MouseY;
	};

	// Mouse scrolled event class
	class PIXELBRAHMA_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}

		// Scroll offset getter functions

		inline float GetXOffset() const { return m_XOffset; }
		inline float GetYOffset() const { return m_YOffset; }

		// Override to string function for logging
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
			return ss.str();
		}

		// Event class descriptions
		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_XOffset, m_YOffset;
	};

	// Mouse button event class
	class PIXELBRAHMA_API MouseButtonEvent : public Event
	{
	public:

		// Mouse button getter function
		inline int GetMouseButton() const { return m_Button; }

		// Event class descriptions
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	protected:
		MouseButtonEvent(int button) : m_Button(button) {}

		int m_Button;
	};

	// Mouse button pressed event
	class PIXELBRAHMA_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

		// Override to string function for logging
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		// Event class descriptions
		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	// Mouse button released event
	class PIXELBRAHMA_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

		// Override to string function for logging
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		// Event class descriptions
		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}