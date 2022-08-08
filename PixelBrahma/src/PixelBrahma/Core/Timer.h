#pragma once

#include <chrono>

namespace PixelBrahma
{
	// Timer class to keep time
	class Timer
	{
	public:
		Timer() { Reset(); }

		// Reset timer function
		void Timer::Reset() { m_Start = std::chrono::high_resolution_clock::now(); }

		// Get time passed function
		float Timer::Elapsed()
		{
			return std::chrono::duration_cast<std::chrono::nanoseconds>
				(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f * 0.001f;
		}

		// Get time passed in milliseconds
		float Timer::ElapsedMillis() { return Elapsed() * 1000.0f; }

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};
}
