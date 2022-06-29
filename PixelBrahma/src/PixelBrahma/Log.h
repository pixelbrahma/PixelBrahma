#pragma once

#include <memory>

#include "Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace PixelBrahma
{
	// Logger wrapper class
	class PIXELBRAHMA_API Log
	{
	public:
		Log();
		~Log();

		// Logger initialization function
		static void Init();

		// Get the Core or client loggers

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Core log macros
#define PB_CORE_TRACE(...) :: PixelBrahma::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define PB_CORE_INFO(...)  :: PixelBrahma::Log::GetCoreLogger()->info(__VA_ARGS__)
#define PB_CORE_WARN(...)  :: PixelBrahma::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define PB_CORE_ERROR(...) :: PixelBrahma::Log::GetCoreLogger()->error(__VA_ARGS__)
#define PB_CORE_FATAL(...) :: PixelBrahma::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define PB_TRACE(...)      :: PixelBrahma::Log::GetClientLogger()->trace(__VA_ARGS__)
#define PB_INFO(...)       :: PixelBrahma::Log::GetClientLogger()->info(__VA_ARGS__)
#define PB_WARN(...)       :: PixelBrahma::Log::GetClientLogger()->warn(__VA_ARGS__)
#define PB_ERROR(...)      :: PixelBrahma::Log::GetClientLogger()->error(__VA_ARGS__)
#define PB_FATAL(...)      :: PixelBrahma::Log::GetClientLogger()->fatal(__VA_ARGS__)