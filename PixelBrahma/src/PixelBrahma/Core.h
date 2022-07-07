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

// Assertion macros

#ifdef PB_ENABLE_ASSERTS
	#define PB_ASSERT(x, ...) {if(!(x)) {PB_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
	#define PB_CORE_ASSERT(x, ...) {if(!(x)) {PB_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
#else
	#define PB_ASSERT(x, ...)
	#define PB_CORE_ASSERT(x, ...)
#endif

// Bit shift macro
#define BIT(x) (1 << x)