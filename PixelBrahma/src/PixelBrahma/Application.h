#pragma once

#include "Core.h"

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
	};

	// To be defined in the client
	Application* CreateApplication();
}