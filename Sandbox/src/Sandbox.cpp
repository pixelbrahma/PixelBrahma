#include <PixelBrahma.h>

// Application instance class
class Sandbox : public PixelBrahma::Application
{
public:
	Sandbox() {}
	~Sandbox() {}
};

// Create application function that passes an application instance to the engine
PixelBrahma::Application* PixelBrahma::CreateApplication()
{
	return new Sandbox();
}