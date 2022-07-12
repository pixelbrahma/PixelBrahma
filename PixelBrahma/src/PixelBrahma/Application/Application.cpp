#include "pbpch.h"
#include "Application.h"

#include "glad/glad.h"

#include "PixelBrahma/Input/Input.h"

namespace PixelBrahma
{
	// Macro for binding events
	#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	// Application should be a singleton
	Application* Application::s_Instance = nullptr;

	Application::Application() 
	{ 
		PB_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application() {}

	// Add layer to the layer stack
	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	// Add overlay to the layer stack
	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	// Event callback function
	void Application::OnEvent(Event& e)
	{
		// Create the event dispatcher and bind it to the OnWindowClose function based on event type
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		// Iterate from the back of the layer stack
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);

			// If event is handled, break out of the loop and dont send the event to the lower layers
			if (e.Handled)
				break;
		}
	}

	// Application run function
	void Application::Run()
	{
		// Application run loop
		while (m_Running)
		{
			// Clear the color buffer
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			// Update each layer in order
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

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