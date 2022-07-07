#pragma once

#include "Event.h"

namespace PixelBrahma
{
	// Window resize event class
	class PIXELBRAHMA_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height) : m_Width(width), m_Height(height) {}

		// Window size getter functions

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

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
	class PIXELBRAHMA_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		// Event class descriptions
		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	// Application tick event
	class PIXELBRAHMA_API AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}
		
		// Event class descriptions
		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	// Application update event
	class PIXELBRAHMA_API AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}

		// Event class descriptions
		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	// Application render event
	class PIXELBRAHMA_API AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}

		// Event class descriptions
		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}