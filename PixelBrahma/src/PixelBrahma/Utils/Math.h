#pragma once

#include <glm/glm.hpp>

namespace PixelBrahma::Math
{
	// Decompose transformation matrix to get translation, rotation and scale values function
	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, 
		glm::vec3& rotation, glm::vec3& scale);

}
