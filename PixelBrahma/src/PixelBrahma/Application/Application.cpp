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

	// Function to convert shader data type to OpenGL base date type
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			// Float data types
			case PixelBrahma::ShaderDataType::Float:  return GL_FLOAT;
			case PixelBrahma::ShaderDataType::Float2: return GL_FLOAT;
			case PixelBrahma::ShaderDataType::Float3: return GL_FLOAT;
			case PixelBrahma::ShaderDataType::Float4: return GL_FLOAT;

			// Matrix data types
			case PixelBrahma::ShaderDataType::Mat3:   return GL_FLOAT;
			case PixelBrahma::ShaderDataType::Mat4:   return GL_FLOAT;

			// Integer data types
			case PixelBrahma::ShaderDataType::Int:    return GL_INT;
			case PixelBrahma::ShaderDataType::Int2:   return GL_INT;
			case PixelBrahma::ShaderDataType::Int3:   return GL_INT;
			case PixelBrahma::ShaderDataType::Int4:   return GL_INT;

			// Boolean data types
			case PixelBrahma::ShaderDataType::Bool:   return GL_BOOL;
		}

		PB_CORE_ASSERT(false, "Unknown shader data type!");
		return 0;
	}

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

		// Vertex attributes array
		float vertices[3 * 7] = 
		{
			-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f
		};

		// Assign vertex buffer data
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		// Define vertex buffer layout and set it
		{
			BufferLayout layout =
			{
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color"    }
			};

			m_VertexBuffer->SetLayout(layout);
		}

		uint32_t index = 0;
		const auto& layout = m_VertexBuffer->GetLayout();

		// Set attribute pointers for element types in the buffer layout
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);

			index++;
		}

		// Indices array
		uint32_t indices[3] = { 0, 1, 2 };

		// Assign index buffer data
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		// Vertex shader source
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
			}
		)";

		// Fragment shader source
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