#pragma once

#include "PixelBrahma/Core.h"
#include "PixelBrahma/Window.h"

#include "PixelBrahma/Layers/LayerStack.h"

#include "PixelBrahma/Events/Event.h"
#include "PixelBrahma/Events/ApplicationEvent.h"

#include "PixelBrahma/ImGui/ImGuiLayer.h"

#include "PixelBrahma/Renderer/Shader.h"
#include "PixelBrahma/Renderer/Buffer.h"
#include "PixelBrahma/Renderer/VertexArray.h"

namespace PixelBrahma
{
	// Application class to act as base class to create application from the client
	class PIXELBRAHMA_API Application
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

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;

		std::shared_ptr<Shader> m_BlueShader;
		std::shared_ptr<VertexArray> m_SquareVA;

	private:
		static Application* s_Instance;
	};

	// To be defined in the client
	Application* CreateApplication();
}