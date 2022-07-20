#pragma once

#include "PixelBrahma/Core.h"
#include "PixelBrahma/Window.h"

#include "PixelBrahma/Layers/LayerStack.h"

#include "PixelBrahma/Events/Event.h"
#include "PixelBrahma/Events/ApplicationEvent.h"

#include "PixelBrahma/ImGui/ImGuiLayer.h"

#include "PixelBrahma/Core/Timestep.h"

namespace PixelBrahma
{
	// Application class to act as base class to create application from the client
	class Application
	{
	public:
		Application();
		virtual ~Application();

		// Run function
		void Run();

		// Event handling and dispatching function
		void OnEvent(Event& e);

		// Add layers functions
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		// Application instance and window getter functions

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }

	private:
		// Window close event function
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
	};

	// To be defined in the client
	Application* CreateApplication();
}