#pragma once

#include "PixelBrahma/Core/Core.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

// Ignore warnings raised inside external headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

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

// Ostream operator overloading function
template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

// Ostream operator overloading function
template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

// Ostream operator overloading function
template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
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
