#include <PixelBrahma.h>

#include "ImGui/imgui.h"

// Example of layer creation
class ExampleLayer : public PixelBrahma::Layer
{
public:
	ExampleLayer() : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		// Create vertex array
		m_VertexArray.reset(PixelBrahma::VertexArray::Create());

		// Vertex attributes array
		float vertices[3 * 7] =
		{
			-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f
		};

		// Create vertex buffer
		std::shared_ptr<PixelBrahma::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(PixelBrahma::VertexBuffer::Create(vertices, sizeof(vertices)));

		// Define vertex buffer layout and set it
		PixelBrahma::BufferLayout layout =
		{
			{ PixelBrahma::ShaderDataType::Float3, "a_Position" },
			{ PixelBrahma::ShaderDataType::Float4, "a_Color"    }
		};

		vertexBuffer->SetLayout(layout);

		// Add vertex buffer to the vertex array
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		// Indices array
		uint32_t indices[3] = { 0, 1, 2 };

		// Create and assign index buffer data
		std::shared_ptr<PixelBrahma::IndexBuffer> indexBuffer;
		indexBuffer.reset(PixelBrahma::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		// Create square vertex array
		m_SquareVA.reset(PixelBrahma::VertexArray::Create());

		// Vertices array for the square
		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		// Create vertex buffer for the square
		std::shared_ptr<PixelBrahma::VertexBuffer> squareVB;
		squareVB.reset(PixelBrahma::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		// Set vertex buffer layout for the square
		squareVB->SetLayout(
			{
				{ PixelBrahma::ShaderDataType::Float3, "a_Position" }
			});

		// Add square vertex buffer to the square vertex array
		m_SquareVA->AddVertexBuffer(squareVB);

		// Square indices
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		// Create and assign square index buffer
		std::shared_ptr<PixelBrahma::IndexBuffer> squareIB;
		squareIB.reset(PixelBrahma::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
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
		m_Shader.reset(new PixelBrahma::Shader(vertexSrc, fragmentSrc));

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
		m_BlueShader.reset(new PixelBrahma::Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
	}

	// Update function override for the layer
	void OnUpdate(PixelBrahma::Timestep timestep) override
	{ 
		// Movement keys

		if (PixelBrahma::Input::IsKeyPressed(PB_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * timestep;
		if (PixelBrahma::Input::IsKeyPressed(PB_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * timestep;

		if (PixelBrahma::Input::IsKeyPressed(PB_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * timestep;
		if (PixelBrahma::Input::IsKeyPressed(PB_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * timestep;

		// Rotation keys

		if (PixelBrahma::Input::IsKeyPressed(PB_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed * timestep;
		if (PixelBrahma::Input::IsKeyPressed(PB_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed * timestep;

		// Clear the color buffer
		PixelBrahma::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		PixelBrahma::RenderCommand::Clear();

		// Set camera transforms
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		// Start rendering the scene
		PixelBrahma::Renderer::BeginScene(m_Camera);

		// Submit objects to the render queue
		PixelBrahma::Renderer::Submit(m_BlueShader, m_SquareVA);
		PixelBrahma::Renderer::Submit(m_Shader, m_VertexArray);

		// Stop rendering the scene
		PixelBrahma::Renderer::EndScene();
	}

	// ImGui render override
	virtual void OnImGuiRender() override {}

	// Event function override for the layer
	void OnEvent(PixelBrahma::Event& event) override {}

	private:
		std::shared_ptr<PixelBrahma::Shader> m_Shader;
		std::shared_ptr<PixelBrahma::VertexArray> m_VertexArray;

		std::shared_ptr<PixelBrahma::Shader> m_BlueShader;
		std::shared_ptr<PixelBrahma::VertexArray> m_SquareVA;

		PixelBrahma::OrthographicCamera m_Camera;

		glm::vec3 m_CameraPosition;
		float m_CameraRotation = 0.0f;
		
		float m_CameraMoveSpeed = 5.0f;
		float m_CameraRotationSpeed = 108.0f;
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