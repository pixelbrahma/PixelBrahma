#pragma once

#include <memory>

#include "PixelBrahma/Core/PlatformDetection.h"

/*
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
*/

// If debug mode enable asserts

#ifdef PB_DEBUG
	#if defined(PB_PLATFORM_WINDOWS)
		#define PB_DEBUGBREAK() __debugbreak()
	#elif defined(PB_PLATFORM_LINUX)
		#include <signal.h>
		#define PB_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	#define PB_ENABLE_ASSERTS
#else
	#define PB_DEBUGBREAK()
#endif

/*
// Assertion macros

#ifdef PB_ENABLE_ASSERTS
	#define PB_ASSERT(x, ...) {if(!(x)) {PB_ERROR("Assertion Failed: {0}", __VA_ARGS__); PB_DEBUGBREAK(); }}
	#define PB_CORE_ASSERT(x, ...) {if(!(x)) {PB_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); PB_DEBUGBREAK(); }}
#else
	#define PB_ASSERT(x, ...)
	#define PB_CORE_ASSERT(x, ...)
#endif
*/

// Expand macro
#define PB_EXPAND_MACRO(x) x

// Make to string macro
#define PB_STRINGIFY_MACRO(x) #x

// Bit shift macro
#define BIT(x) (1 << x)

// Bind event macro
#define PB_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) \
	{ return this->fn(std::forward<decltype(args)>(args)...); }

namespace PixelBrahma
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}


// Includes

#include "PixelBrahma/Log/Log.h"
#include "Assert.h"
