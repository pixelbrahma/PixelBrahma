#pragma once

#include "PixelBrahma/Core/PlatformDetection.h"

#ifdef PB_PLATFORM_WINDOWS
	#ifndef NOMINMAX
		// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
		#define NOMINMAX
	#endif
#endif

// Precompiled Headers

// Headers

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

// Data structures and streams

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

// Defined headers

#include "PixelBrahma/Core/Core.h"
#include "PixelBrahma/Log/Log.h"

// Instrumentation
#include "PixelBrahma/Debug/Instrumentor.h"

#ifdef PB_PLATFORM_WINDOWS
	#include <Windows.h>
#endif
