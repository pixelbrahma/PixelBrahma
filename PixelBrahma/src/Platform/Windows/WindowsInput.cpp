#include "pbpch.h"
#include "WindowsInput.h"

#include "PixelBrahma/Application/Application.h"

#include <GLFW/glfw3.h>

namespace PixelBrahma
{
	// Input manager instance 
	Scope<Input> Input::s_Instance = CreateScope<WindowsInput>();

	// Key pressed input state implementation
	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		// From application instance, get window and from the window, get platform specific window
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	// Mouse pressed input state implementation
	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		// From application instance, get window and from the window, get platform specific window
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	// Mouse position input implementation
	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		// From application instance, get window and from the window, get platform specific window
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}

	// Mouse position X getter implementation
	float WindowsInput::GetMouseXImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return x;
	}

	// Mouse position Y getter implementation
	float WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return y;
	}
}