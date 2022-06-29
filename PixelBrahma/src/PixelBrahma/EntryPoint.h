#pragma once

#ifdef PB_PLATFORM_WINDOWS

// Function defined elsewhere that returns an engine application
extern PixelBrahma::Application* PixelBrahma::CreateApplication();

int main(int argc, char** argv)
{
	printf("PB Engine\n");

	auto application = PixelBrahma::CreateApplication();
	application->Run();
	delete application;
}

#endif // PB_PLATFORM_WINDOWS