#include <PixelBrahma.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "ImGui/imgui.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

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
		PixelBrahma::Ref<PixelBrahma::VertexBuffer> vertexBuffer;
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
		PixelBrahma::Ref<PixelBrahma::IndexBuffer> indexBuffer;
		indexBuffer.reset(PixelBrahma::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		// Create square vertex array
		m_SquareVA.reset(PixelBrahma::VertexArray::Create());

		// Vertices array for the square
		float squareVertices[5 * 4] = {
			// Positions            // Texture coordinates
			-0.5f, -0.5f, 0.0f,     0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,     1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f,     1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,     0.0f, 1.0f
		};

		// Create vertex buffer for the square
		PixelBrahma::Ref<PixelBrahma::VertexBuffer> squareVB;
		squareVB.reset(PixelBrahma::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		// Set vertex buffer layout for the square
		squareVB->SetLayout(
			{
				{ PixelBrahma::ShaderDataType::Float3, "a_Position" },
				{ PixelBrahma::ShaderDataType::Float2, "a_TexCoord" }
			});

		// Add square vertex buffer to the square vertex array
		m_SquareVA->AddVertexBuffer(squareVB);

		// Square indices
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		// Create and assign square index buffer
		PixelBrahma::Ref<PixelBrahma::IndexBuffer> squareIB;
		squareIB.reset(PixelBrahma::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		//// Shaders for triangle ////

		// Vertex shader source for the triangle
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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
		m_Shader.reset(PixelBrahma::Shader::Create(vertexSrc, fragmentSrc));

		//// Shaders for square ////

		// Vertex shader source for the square
		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		// Fragment shader source for the square
		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		// Create new shader pointer from vertex and fragment shader sources for square
		m_FlatColorShader.reset(PixelBrahma::Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));

		// Vertex shader source for textured object
		std::string textureShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		// Fragment shader source for textured object
		std::string textureShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec2 v_TexCoord;
			
			uniform sampler2D u_Texture;

			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";

		// Create new shader from sources for texture object shader
		m_TextureShader.reset(PixelBrahma::Shader::Create(textureShaderVertexSrc, textureShaderFragmentSrc));

		// Create 2D textures from asset PNG image in file path
		m_Texture = PixelBrahma::Texture2D::Create("Assets/Textures/CheckerBoard.png");
		m_LogoTexture = PixelBrahma::Texture2D::Create("Assets/Textures/Logo.png");

		// Bind and set shader uniforms for the textured objects
		std::dynamic_pointer_cast<PixelBrahma::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<PixelBrahma::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
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

		// Scale matrix
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		// Bind and set shader uniforms for the square
		std::dynamic_pointer_cast<PixelBrahma::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<PixelBrahma::OpenGLShader>(m_FlatColorShader)->
			UploadUniformFloat3("u_Color", m_SquareColor);

		// Create tile map by setting tranforms of the sqaures
		for(int y = 0; y < 20; y++)
			for (int x = 0; x < 20; x++)
			{
				// Transform position
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);

				// Translation matrix
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

				// Submit the object for rendering 
				PixelBrahma::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}

		// Bind texture and submit to the render queue
		m_Texture->Bind();
		PixelBrahma::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Bind logo and submit to the render queue
		m_LogoTexture->Bind();
		PixelBrahma::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Submit triangle object to the render queue
		//PixelBrahma::Renderer::Submit(m_Shader, m_VertexArray);

		// Stop rendering the scene
		PixelBrahma::Renderer::EndScene();
	}

	// ImGui render override
	virtual void OnImGuiRender() override 
	{
		// Create an ImGui color picker to pick the color of the squares

		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	// Event function override for the layer
	void OnEvent(PixelBrahma::Event& event) override {}

	private:

		// Triangle object
		PixelBrahma::Ref<PixelBrahma::Shader> m_Shader;
		PixelBrahma::Ref<PixelBrahma::VertexArray> m_VertexArray;

		// Square object
		PixelBrahma::Ref<PixelBrahma::Shader> m_FlatColorShader;
		PixelBrahma::Ref<PixelBrahma::VertexArray> m_SquareVA;

		// Textured object
		PixelBrahma::Ref<PixelBrahma::Shader> m_TextureShader;
		PixelBrahma::Ref<PixelBrahma::Texture2D> m_Texture;
		
		// Logo
		PixelBrahma::Ref<PixelBrahma::Texture2D> m_LogoTexture;

		// Camera object
		PixelBrahma::OrthographicCamera m_Camera;

		// Transform properties
		glm::vec3 m_CameraPosition;
		float m_CameraRotation = 0.0f;
		
		// Movement and rotation speed
		float m_CameraMoveSpeed = 5.0f;
		float m_CameraRotationSpeed = 108.0f;

		glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
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