#include <PixelBrahma.h>

#include "ImGui/imgui.h"

// Example of layer creation
class ExampleLayer : public PixelBrahma::Layer
{
public:
	ExampleLayer() : Layer("Example") {}

	// Update function override for the layer
	void OnUpdate() override 
	{ 
		// If tab key is pressed, log it
		if (PixelBrahma::Input::IsKeyPressed(PB_KEY_TAB))
			PB_TRACE("Tab key is pressed (poll)!");
	}

	// ImGui render override
	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	// Event function override for the layer
	void OnEvent(PixelBrahma::Event& event) override 
	{ 
		// If key pressed event occurs, log it
		if (event.GetEventType() == PixelBrahma::EventType::KeyPressed)
		{
			PixelBrahma::KeyPressedEvent& e = (PixelBrahma::KeyPressedEvent&)event;
			if (e.GetKeyCode() == PB_KEY_TAB)
				PB_TRACE("Tab key is pressed (event)!");
			PB_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};

// Application instance class
class Sandbox : public PixelBrahma::Application
{
public:
	Sandbox() { PushLayer(new ExampleLayer()); }
	~Sandbox() {}
};

// Create application function that passes an application instance to the engine
PixelBrahma::Application* PixelBrahma::CreateApplication()
{
	return new Sandbox();
}