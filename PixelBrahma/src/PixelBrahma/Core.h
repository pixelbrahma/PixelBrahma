#pragma once

#include <memory>

// Dll import or export macros

#ifdef PB_PLATFORM_WINDOWS
	#if PB_DYNAMIC_LINK
		#ifdef PB_BUILD_DLL
			#define PIXELBRAHMA_API __declspec(dllexport)
		#else
			#define PIXELBRAHMA_API __declspec(dllimport)
		#endif // PB_BUILD_DLL
	#else
		#define PIXELBRAHMA_API
	#endif
#else
	#error PixelBrahma Engine only supports Windows!
#endif // PB_PLATFORM_WINDOWS

// If debug mode enable asserts

#ifdef PB_DEBUG
	#define PB_ENABLE_ASSERTS
#endif

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

// Bind event macro
#define PB_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace PixelBrahma
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}