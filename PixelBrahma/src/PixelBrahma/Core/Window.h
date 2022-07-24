#pragma once

#include "pbpch.h"

#include "PixelBrahma/Core/Core.h"
#include "PixelBrahma/Events/Event.h"

namespace PixelBrahma
{

	// Structure of window properties
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "PixelBrahma Engine",
			unsigned int width = 1280,
			unsigned int height = 720) : Title(title), Width(width), Height(height) {}
	};

	// Interface representing a desktop based window - Pure virtual class
	class PIXELBRAHMA_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		// Virtual destructor
		virtual ~Window() = default;

		// Virtual on update function
		virtual void OnUpdate() = 0;

		// Virtual window dimensions getter functions

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void* GetNativeWindow() const = 0;

		// Create window function - Must be implemented per platform
		static Window* Create(const WindowProps& props = WindowProps());
	};
}