#pragma once

#ifdef PB_PLATFORM_WINDOWS

// Function defined elsewhere that returns an engine application
extern PixelBrahma::Application* PixelBrahma::CreateApplication();

// Main function - Program entry point
int main(int argc, char** argv)
{
	std::printf("PB ENGINE!\n");

	// Initialaize logger
	PixelBrahma::Log::Init();
	int a = 5;
	PB_CORE_WARN("Core");
	PB_INFO("Client {0}", a);

	// Call create application function defined in the client
	auto application = PixelBrahma::CreateApplication();

	// Call the application run function
	application->Run();

	// Delete application instance
	delete application;
}

#endif // PB_PLATFORM_WINDOWS