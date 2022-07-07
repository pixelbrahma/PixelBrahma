#include "pbpch.h"
#include "Application.h"

#include "PixelBrahma/Events/ApplicationEvent.h"

#include "GLFW/glfw3.h"

namespace PixelBrahma
{
	Application::Application() { m_Window = std::unique_ptr<Window>(Window::Create()); }
	Application::~Application() {}

	// Application run function
	void Application::Run()
	{
		// Application run loop
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			// Call the update function of the window
			m_Window->OnUpdate();
		}
	}
}