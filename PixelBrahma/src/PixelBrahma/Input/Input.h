#pragma once

#include "PixelBrahma/Input/MouseCodes.h"
#include "PixelBrahma/Input/KeyCodes.h"

#include <glm/glm.hpp>

namespace PixelBrahma
{
	// Input manager interface
	class Input
	{
	public:

		// Static input state getter functions

		static bool IsKeyPressed(KeyCode key);
		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}
