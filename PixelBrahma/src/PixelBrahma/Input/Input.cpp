#include "pbpch.h"
#include "Input.h"

#ifdef PB_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsInput.h"
#endif // PB_PLATFORM_WINDOWS

namespace PixelBrahma
{
	// Create Input manager instance
	Scope<Input> Input::s_Instance = Input::Create();

	// Input manager create function
	Scope<Input> Input::Create()
	{
	#ifdef PB_PLATFORM_WINDOWS
		return CreateScope<WindowsInput>();
	#else
		PB_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
	#endif
	}
}