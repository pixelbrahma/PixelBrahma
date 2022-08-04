#pragma once

#include <sstream>

#include "PixelBrahma/Core/Core.h"
#include "PixelBrahma/Events/Event.h"

namespace PixelBrahma
{

	// Structure of window properties
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "PixelBrahma Engine",
			uint32_t width = 1600,
			uint32_t height = 900) : Title(title), Width(width), Height(height) {}
	};

	// Interface representing a desktop based window - Pure virtual class
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		// Virtual destructor
		virtual ~Window() = default;

		// Virtual on update function
		virtual void OnUpdate() = 0;

		// Virtual window dimensions getter functions

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Window attributes

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void* GetNativeWindow() const = 0;

		// Create window function - Must be implemented per platform
		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}
