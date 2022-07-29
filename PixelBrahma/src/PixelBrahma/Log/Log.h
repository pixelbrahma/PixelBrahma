#pragma once

#include "PixelBrahma/Core/Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace PixelBrahma
{
	// Logger wrapper class
	class Log
	{
	public:
		Log();
		~Log();

		// Logger initialization function
		static void Init();

		// Get the Core or client loggers

		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}

// Core log macros
#define PB_CORE_TRACE(...)    :: PixelBrahma::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define PB_CORE_INFO(...)     :: PixelBrahma::Log::GetCoreLogger()->info(__VA_ARGS__)
#define PB_CORE_WARN(...)     :: PixelBrahma::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define PB_CORE_ERROR(...)    :: PixelBrahma::Log::GetCoreLogger()->error(__VA_ARGS__)
#define PB_CORE_CRITICAL(...) :: PixelBrahma::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define PB_TRACE(...)         :: PixelBrahma::Log::GetClientLogger()->trace(__VA_ARGS__)
#define PB_INFO(...)          :: PixelBrahma::Log::GetClientLogger()->info(__VA_ARGS__)
#define PB_WARN(...)          :: PixelBrahma::Log::GetClientLogger()->warn(__VA_ARGS__)
#define PB_ERROR(...)         :: PixelBrahma::Log::GetClientLogger()->error(__VA_ARGS__)
#define PB_CRITICAL(...)      :: PixelBrahma::Log::GetClientLogger()->critical(__VA_ARGS__)