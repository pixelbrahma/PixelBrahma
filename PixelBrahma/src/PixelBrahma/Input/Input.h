#pragma once

#include "PixelBrahma/Core/Core.h"
#include "PixelBrahma/Input/KeyCodes.h"
#include "PixelBrahma/Input/MouseCodes.h"

namespace PixelBrahma
{
	// Input manager interface
	class Input
	{
	public:

		// Static input state getter functions

		static bool IsKeyPressed(KeyCode key);
		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}