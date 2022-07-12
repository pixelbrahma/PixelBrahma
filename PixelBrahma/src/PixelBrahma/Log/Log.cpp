#include "pbpch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace PixelBrahma
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	Log::Log() {}

	Log::~Log() {}

	// Initialize the Logger
	void Log::Init()
	{
		// Time stamp, Logger name, Message, In the required color
		spdlog::set_pattern("%^[%T] %n: %v%$");

		// Core logger properties
		s_CoreLogger = spdlog::stdout_color_mt("PIXELBRAHMA");
		s_CoreLogger->set_level(spdlog::level::trace);

		// Client logger properties
		s_ClientLogger = spdlog::stdout_color_mt("APPLICATION");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}