#pragma once

#include "PixelBrahma/Core/Core.h"
#include "PixelBrahma/Input/KeyCodes.h"
#include "PixelBrahma/Input/MouseCodes.h"

namespace PixelBrahma
{
	// Input manager interface
	class Input
	{
		// Make Input class a true singleton

	protected:
		Input() = default;

	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

	public:

		// Static input state getter functions

		static bool IsKeyPressed(KeyCode key) { return s_Instance->IsKeyPressedImpl(key); }
		static bool IsMouseButtonPressed(MouseCode button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		static float GetMouseY() { return s_Instance->GetMouseYImpl(); }

		// Create Input manager
		static Scope<Input> Create();

	protected:

		// Virtual platform specific implementation functions

		virtual bool IsKeyPressedImpl(KeyCode keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(MouseCode button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

	private:
		static Scope<Input> s_Instance;
	};
}