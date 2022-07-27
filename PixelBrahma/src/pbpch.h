#pragma once

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

#include "PixelBrahma/Log/Log.h"

// Instrumentation
#include "PixelBrahma/Debug/Instrumentor.h"

#ifdef PB_PLATFORM_WINDOWS
	#include <Windows.h>
#endif