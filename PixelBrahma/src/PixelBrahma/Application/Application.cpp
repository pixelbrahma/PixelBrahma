#include "pbpch.h"
#include "Application.h"

#include "glad/glad.h"

#include "PixelBrahma/Input/Input.h"

namespace PixelBrahma
{
	// Macro for binding events
	#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	// Application should be a singleton
	Application* Application::s_Instance = nullptr;

	Application::Application() 
	{ 
		// Set application instance
		PB_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		// Create window and set reference and event callback
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		// Create ImGui layer push it to the layer stack
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		// Generate and bind vertex array
		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		// Vertex positions array
		float vertices[3 * 3] =
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		// Assign vertex buffer data
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		// Vertex attribute arrray and pointer
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		// Indices array
		uint32_t indices[3] = { 0, 1, 2 };

		// Assign index buffer data
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		// Vertex shader source
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
			}
		)";

		// Fragment shader source
		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
			}
		)";

		// Create new unique shader pointer from vertex and fragment shader sources
		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
	}

	Application::~Application() {}

	// Add layer to the layer stack
	void Application::PushLayer(Layer* layer) { m_LayerStack.PushLayer(layer); }

	// Add overlay to the layer stack
	void Application::PushOverlay(Layer* overlay) { m_LayerStack.PushOverlay(overlay); }

	// Event callback function
	void Application::OnEvent(Event& e)
	{
		// Create the event dispatcher and bind it to the OnWindowClose function based on event type
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		// Iterate from the back of the layer stack
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);

			// If event is handled, break out of the loop and dont send the event to the lower layers
			if (e.Handled)
				break;
		}
	}

	// Application run function
	void Application::Run()
	{
		// Application run loop
		while (m_Running)
		{
			// Clear the color buffer
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			// Bind the shader
			m_Shader->Bind();

			// Draw the elements of the vertices array
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			// Update each layer in order
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			// ImGui layer run functionality

			m_ImGuiLayer->Begin();

			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();

			m_ImGuiLayer->End();

			// Call the update function of the window
			m_Window->OnUpdate();
		}
	}

	// Window close event function
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}