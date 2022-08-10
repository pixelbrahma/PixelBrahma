#pragma once

#include "PixelBrahma/Core/Core.h"
#include "PixelBrahma/Core/Window.h"
#include "PixelBrahma/Core/Timestep.h"

#include "PixelBrahma/Layers/LayerStack.h"

#include "PixelBrahma/Events/Event.h"
#include "PixelBrahma/Events/ApplicationEvent.h"

#include "PixelBrahma/ImGui/ImGuiLayer.h"


// Run loop main function forward declaration to make it accessible only on the engine side
int main(int argc, char** argv);

namespace PixelBrahma
{
	// Application command line arguments structure
	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			PB_CORE_ASSERT(index < Count);
			return Args[index];
		}
	};

	// Application class to act as base class to create application from the client
	class Application
	{
	public:
		Application(const std::string& name = "PB Application", 
			ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		virtual ~Application();

		// Event handling and dispatching function
		void OnEvent(Event& e);

		// Add layers functions
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		// Close application function
		void Close();

		// Get ImGuiLayer function
		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		// Application instance and window getter functions

		Window& GetWindow() { return *m_Window; }
		static Application& Get() { return *s_Instance; }

		// Application command line arguments getter function
		ApplicationCommandLineArgs GetCommandLineArgs() const { return m_CommandLineArgs; }

	private:
		// Run function
		void Run();

		// Window event functions

		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);

	private:
		ApplicationCommandLineArgs m_CommandLineArgs;
		
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;

		Scope<Window> m_Window;
		
		bool m_Running = true;
		bool m_Minimized = false;
		
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	// To be defined in the client
	Application* CreateApplication(ApplicationCommandLineArgs args);
}
