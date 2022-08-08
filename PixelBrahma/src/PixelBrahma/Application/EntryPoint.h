#pragma once

#include "PixelBrahma/Core/Core.h"
#include "Application.h"

#ifdef PB_PLATFORM_WINDOWS

// Function defined elsewhere that returns an engine application
extern PixelBrahma::Application* PixelBrahma::CreateApplication(ApplicationCommandLineArgs args);

// Main function - Program entry point
int main(int argc, char** argv)
{
	std::printf("PB ENGINE!\n");

	// Initialaize logger
	PixelBrahma::Log::Init();

	PB_PROFILE_BEGIN_SESSION("Startup", "PBProfile-Startup.json");
	// Call create application function defined in the client
	auto application = PixelBrahma::CreateApplication({ argc, argv });
	PB_PROFILE_END_SESSION();

	PB_PROFILE_BEGIN_SESSION("Runtime", "PBProfile-Runtime.json");
	// Call the application run function
	application->Run();
	PB_PROFILE_END_SESSION();

	PB_PROFILE_BEGIN_SESSION("Shutdown", "PBProfile-Shutdown.json");
	// Delete application instance
	delete application;
	PB_PROFILE_END_SESSION();
}

#endif // PB_PLATFORM_WINDOWS
