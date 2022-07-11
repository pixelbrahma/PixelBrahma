#include <PixelBrahma.h>

// Example of layer creation
class ExampleLayer : public PixelBrahma::Layer
{
public:
	ExampleLayer() : Layer("Example") {}

	// Update function override for the layer
	void OnUpdate() override { PB_TRACE("ExampleLayer::Update"); }

	// Event function override for the layer
	void OnEvent(PixelBrahma::Event& event) override { PB_INFO("{0}", event); }
};

// Application instance class
class Sandbox : public PixelBrahma::Application
{
public:
	Sandbox() 
	{ 
		PushLayer(new ExampleLayer()); 
		PushOverlay(new PixelBrahma::ImGuiLayer());
	}
	~Sandbox() {}
};

// Create application function that passes an application instance to the engine
PixelBrahma::Application* PixelBrahma::CreateApplication()
{
	return new Sandbox();
}