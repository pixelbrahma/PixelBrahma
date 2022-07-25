#include "Sandbox2D.h"

#include "ImGui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true) {}

// Layer on attach function
void Sandbox2D::OnAttach()
{
	// Create vertex array
	m_SquareVA = PixelBrahma::VertexArray::Create();

	// Vertices
	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	// Create and add Vertex buffer

	PixelBrahma::Ref<PixelBrahma::VertexBuffer> squareVB;
	squareVB.reset(PixelBrahma::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	squareVB->SetLayout({
		{ PixelBrahma::ShaderDataType::Float3, "a_Position" }
		});
	m_SquareVA->AddVertexBuffer(squareVB);

	// Indices
	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

	// Create and set index buffer
	PixelBrahma::Ref<PixelBrahma::IndexBuffer> squareIB;
	squareIB.reset(PixelBrahma::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	m_SquareVA->SetIndexBuffer(squareIB);

	// Load shader from file path
	m_FlatColorShader = PixelBrahma::Shader::Create("Assets/Shaders/FlatColor.glsl");
}

// Layer on detach function
void Sandbox2D::OnDetach() {}

// Layer update function
void Sandbox2D::OnUpdate(PixelBrahma::Timestep timestep)
{
	// Call camera update function
	m_CameraController.OnUpdate(timestep);

	// Clear the color buffer
	PixelBrahma::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	PixelBrahma::RenderCommand::Clear();

	// Start rendering the scene
	PixelBrahma::Renderer::BeginScene(m_CameraController.GetCamera());

	// Bind and set shader uniforms for the square
	std::dynamic_pointer_cast<PixelBrahma::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<PixelBrahma::OpenGLShader>(m_FlatColorShader)->
		UploadUniformFloat4("u_Color", m_SquareColor);

	// Submit draw call to the renderer
	PixelBrahma::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	// Stop rendering the scene
	PixelBrahma::Renderer::EndScene();
}

// ImGui render function
void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

// Layer event handler function
void Sandbox2D::OnEvent(PixelBrahma::Event& event)
{
	m_CameraController.OnEvent(event);
}