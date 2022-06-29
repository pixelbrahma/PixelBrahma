#pragma once

#include "Core.h"

namespace PixelBrahma
{
	class PIXELBRAHMA_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in the client
	Application* CreateApplication();
}