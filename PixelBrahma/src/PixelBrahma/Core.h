#pragma once

// Dll import or export macros

#ifdef PB_PLATFORM_WINDOWS
	#ifdef PB_BUILD_DLL
		#define PIXELBRAHMA_API __declspec(dllexport)
	#else
		#define PIXELBRAHMA_API __declspec(dllimport)
	#endif // PB_BUILD_DLL
#else
	#error PixelBrahma Engine only supports Windows!
#endif // PB_PLATFORM_WINDOWS

#define BIT(x) (1 << x)