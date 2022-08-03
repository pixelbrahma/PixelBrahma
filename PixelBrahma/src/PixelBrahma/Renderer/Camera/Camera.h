#pragma once

#include <glm/glm.hpp>

namespace PixelBrahma
{
	// Generic camera class used mainly to create camera components
	class Camera
	{
	public:
		Camera(const glm::mat4& projection) : m_Projection(projection) {}

		// Get projection function
		const glm::mat4& GetProjection() const { return m_Projection; }

	private:
		glm::mat4 m_Projection;
	};
}