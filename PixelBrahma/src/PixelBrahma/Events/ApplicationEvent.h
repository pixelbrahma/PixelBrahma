#pragma once

#include "Event.h"

namespace PixelBrahma
{
	// Window resize event class
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height) : m_Width(width), m_Height(height) {}

		// Window size getter functions

		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }

		// Override to string function for logging
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		// Event class descriptions
		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		unsigned int m_Width, m_Height;
	};

	// Window close event class
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		// Event class descriptions
		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	// Application tick event
	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;
		
		// Event class descriptions
		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	// Application update event
	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;

		// Event class descriptions
		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	// Application render event
	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;

		// Event class descriptions
		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}