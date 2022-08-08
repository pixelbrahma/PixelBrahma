#include <PixelBrahma.h>
#include <PixelBrahma/Application/EntryPoint.h>

#include "Sandbox2D.h"
#include "ExampleLayer.h"

// Application instance class
class Sandbox : public PixelBrahma::Application
{
public:
	Sandbox(PixelBrahma::ApplicationCommandLineArgs args) 
	{ 
		//PushLayer(new ExampleLayer()); 
		PushLayer(new Sandbox2D());
	}
	~Sandbox() {}
};

// Create application function that passes an application instance to the engine
PixelBrahma::Application* PixelBrahma::CreateApplication(PixelBrahma::ApplicationCommandLineArgs args)
{
	return new Sandbox(args);
}
