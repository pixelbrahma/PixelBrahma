#include "pbpch.h"
#include "Application.h"

#include "PixelBrahma/Log/Log.h"
#include "PixelBrahma/Input/Input.h"
#include "PixelBrahma/Renderer/Renderer.h"
#include "PixelBrahma/Utils/PlatformUtils.h"

namespace PixelBrahma
{
	// Application should be a singleton
	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& specification)
		: m_Specification(specification)
	{ 
		// Profiling
		PB_PROFILE_FUNCTION();

		// Set application instance
		PB_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		// Create window and set reference, working directory and event callback
		if (!m_Specification.WorkingDirectory.empty())
			std::filesystem::current_path(m_Specification.WorkingDirectory);

		m_Window = Window::Create(WindowProps(m_Specification.Name));
		m_Window->SetEventCallback(PB_BIND_EVENT_FN(Application::OnEvent));

		// Initialize the renderer
		Renderer::Init();

		// Create ImGui layer push it to the layer stack
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application() 
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		// Close the renderer and release resources
		Renderer::Shutdown();
	}

	// Add layer to the layer stack
	void Application::PushLayer(Layer* layer) 
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer); 
		layer->OnAttach();
	}

	// Add overlay to the layer stack
	void Application::PushOverlay(Layer* overlay) 
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay); 
		overlay->OnAttach();
	}

	// Close the application
	void Application::Close()
	{
		m_Running = false;
	}

	// Event callback function
	void Application::OnEvent(Event& event)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		// Create the event dispatcher and bind it to the event handler functions based on event type
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(PB_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(PB_BIND_EVENT_FN(Application::OnWindowResize));

		// Iterate from the back of the layer stack
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			// If event is handled, break out of the loop and dont send the event to the lower layers
			if (event.Handled)
				break;

			(*it)->OnEvent(event);
		}
	}

	// Application run function
	void Application::Run()
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		// Application run loop
		while (m_Running)
		{
			// Profiling
			PB_PROFILE_SCOPE("RunLoop");

			// Timestep calculation

			float time = Time::GetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				{
					// Profiling
					PB_PROFILE_SCOPE("LayerStack OnUpdate");

					// Update each layer in order
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}

				// ImGui layer run functionality

				m_ImGuiLayer->Begin();

				{
					// Profiling
					PB_PROFILE_SCOPE("LayerStack OnImGuiRender");

					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
				}

				m_ImGuiLayer->End();
			}

			// Call the update function of the window
			m_Window->OnUpdate();
		}
	}

	// Window close event handler
	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}

	// Window resize event handler
	bool Application::OnWindowResize(WindowResizeEvent& event)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		if (event.GetWidth() == 0 || event.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		
		// Call renderer window resize event handler
		Renderer::OnWindowResize(event.GetWidth(), event.GetHeight());

		return false;
	}
}
