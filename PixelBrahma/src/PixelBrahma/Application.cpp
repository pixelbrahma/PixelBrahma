#include "pbpch.h"
#include "Application.h"

#include "PixelBrahma/Log.h"
#include "PixelBrahma/Events/ApplicationEvent.h"

namespace PixelBrahma
{
	Application::Application() {}
	Application::~Application() {}

	// Application run function
	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		PB_TRACE(e);

		while (true);
	}
}