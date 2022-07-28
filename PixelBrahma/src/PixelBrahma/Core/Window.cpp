#include "pbpch.h"
#include "Window.h"

#ifdef PB_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif

namespace PixelBrahma
{
	// Window create function
	Scope<Window> Window::Create(const WindowProps& props)
	{
	#ifdef PB_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
	#else
		PB_CORE_ASSERT(false, "Uknown platform!");
		return nullptr;
	#endif
	}
}