#pragma once

#include "PixelBrahma/Core/Core.h"
#include "PixelBrahma/Log/Log.h"

#include <filesystem>

#ifdef PB_ENABLE_ASSERTS

	// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
	// provide support for custom formatting by concatenating the formatting string instead of having 
	// the format inside the default message
	#define PB_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) \
		{ PB##type##ERROR(msg, __VA_ARGS__); PB_DEBUGBREAK(); } }
	#define PB_INTERNAL_ASSERT_WITH_MSG(type, check, ...) \
		PB_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define PB_INTERNAL_ASSERT_NO_MSG(type, check) \
		PB_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", PB_STRINGIFY_MACRO(check), \
		std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define PB_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define PB_INTERNAL_ASSERT_GET_MACRO(...) \
		PB_EXPAND_MACRO( PB_INTERNAL_ASSERT_GET_MACRO_NAME\
		(__VA_ARGS__, PB_INTERNAL_ASSERT_WITH_MSG, PB_INTERNAL_ASSERT_NO_MSG) )

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define PB_ASSERT(...) PB_EXPAND_MACRO( PB_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define PB_CORE_ASSERT(...) PB_EXPAND_MACRO( PB_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define PB_ASSERT(...)
	#define PB_CORE_ASSERT(...)
#endif
