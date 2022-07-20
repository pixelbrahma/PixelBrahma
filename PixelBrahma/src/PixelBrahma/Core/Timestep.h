#pragma once

namespace PixelBrahma
{
	// Class that handles delta time information
	class Timestep
	{
	public:
		Timestep(float time = 0.0f) : m_Time(time) {}

		// Operator overloading for float casting
		operator float() const { return m_Time; }

		// Time getter functions

		float GetSeconds() const { return m_Time; }
		float GetMilliSeconds() const { return m_Time * 1000.0f; }

	private:
		float m_Time;
	};
}