#include "pbpch.h"
#include "PixelBrahma/Input/Input.h"

#include "PixelBrahma/Application/Application.h"

#include <GLFW/glfw3.h>

namespace PixelBrahma
{
	// Key pressed input state implementation
	bool Input::IsKeyPressed(const KeyCode key)
	{
		// From application instance, get window and from the window, get platform specific window
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetKey(window, static_cast<int32_t>(key));
		return state == GLFW_PRESS;
	}

	// Mouse pressed input state implementation
	bool Input::IsMouseButtonPressed(const MouseCode button)
	{
		// From application instance, get window and from the window, get platform specific window
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}

	// Mouse position input implementation
	glm::vec2 Input::GetMousePosition()
	{
		// From application instance, get window and from the window, get platform specific window
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}

	// Mouse position X getter implementation
	float Input::GetMouseX()
	{
		return GetMousePosition().x;
	}

	// Mouse position Y getter implementation
	float Input::GetMouseY()
	{
		return GetMousePosition().y;
	}
}
