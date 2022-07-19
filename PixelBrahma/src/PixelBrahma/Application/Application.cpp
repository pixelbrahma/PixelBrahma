#include "pbpch.h"
#include "Application.h"

#include "PixelBrahma/Input/Input.h"
#include "PixelBrahma/Renderer/Renderer.h"

namespace PixelBrahma
{
	// Macro for binding events
	#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	// Application should be a singleton
	Application* Application::s_Instance = nullptr;

	Application::Application() : m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
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

		// Create vertex array
		m_VertexArray.reset(VertexArray::Create());

		// Vertex attributes array
		float vertices[3 * 7] = 
		{
			-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f
		};

		// Create vertex buffer
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		// Define vertex buffer layout and set it
		BufferLayout layout =
		{
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color"    }
		};

		vertexBuffer->SetLayout(layout);

		// Add vertex buffer to the vertex array
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		// Indices array
		uint32_t indices[3] = { 0, 1, 2 };

		// Create and assign index buffer data
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		// Create square vertex array
		m_SquareVA.reset(VertexArray::Create());

		// Vertices array for the square
		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		// Create vertex buffer for the square
		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		// Set vertex buffer layout for the square
		squareVB->SetLayout(
			{
				{ ShaderDataType::Float3, "a_Position" }
			});

		// Add square vertex buffer to the square vertex array
		m_SquareVA->AddVertexBuffer(squareVB);

		// Square indices
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		// Create and assign square index buffer
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		//// Shaders for triangle ////

		// Vertex shader source for the triangle
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		// Fragment shader source for the triangle
		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		// Create new shader pointer from vertex and fragment shader sources for triangle
		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

		//// Shaders for square ////

		// Vertex shader source for the square
		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
			}
		)";

		// Fragment shader source for the square
		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		// Create new shader pointer from vertex and fragment shader sources for square
		m_BlueShader.reset(new Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
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
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();
			
			// Set camera transforms
			m_Camera.SetPosition({ 0.5f, 0.5f, 0.0f });
			m_Camera.SetRotation(45.0f);

			// Start rendering the scene
			Renderer::BeginScene(m_Camera);

			// Submit objects to the render queue
			Renderer::Submit(m_BlueShader, m_SquareVA);
			Renderer::Submit(m_Shader, m_VertexArray);

			// Stop rendering the scene
			Renderer::EndScene();

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