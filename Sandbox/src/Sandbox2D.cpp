#include "Sandbox2D.h"

#include <ImGui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true) {}

// Layer on attach function
void Sandbox2D::OnAttach() 
{ 
	// Load texture from file path
	m_CheckerboardTexture = PixelBrahma::Texture2D::Create("Assets/Textures/CheckerBoard.png");
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

	// Start rendering the 2D scene
	PixelBrahma::Renderer2D::BeginScene(m_CameraController.GetCamera());

	// Draw quads
	PixelBrahma::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	PixelBrahma::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
	PixelBrahma::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture);

	// Stop rendering the scene
	PixelBrahma::Renderer2D::EndScene();
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