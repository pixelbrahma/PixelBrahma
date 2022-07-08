#pragma once

#include "Core.h"
#include "Window.h"
#include "Layers/LayerStack.h"
#include "Events/Event.h"
#include "PixelBrahma/Events/ApplicationEvent.h"

namespace PixelBrahma
{
	// Application class to act as base class to create application from the client
	class PIXELBRAHMA_API Application
	{
	public:
		Application();
		virtual ~Application();

		// Run function
		void Run();

		// Event handling and dispatching function
		void OnEvent(Event& e);

		// Add layers functions
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

	private:
		// Window close event function
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	// To be defined in the client
	Application* CreateApplication();
}