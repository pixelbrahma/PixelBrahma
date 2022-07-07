#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

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

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// To be defined in the client
	Application* CreateApplication();
}