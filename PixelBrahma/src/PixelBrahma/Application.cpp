#include "pbpch.h"
#include "Application.h"

#include "GLFW/glfw3.h"

namespace PixelBrahma
{
	// Macro for binding events
	#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application::Application() 
	{ 
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application() {}

	// Event callback function
	void Application::OnEvent(Event& e)
	{
		// Create the event dispatcher and bind it to the OnWindowClose function
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		PB_CORE_TRACE("{0}", e);
	}

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

	// Window close event function
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}