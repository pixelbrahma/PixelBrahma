#include "Sandbox2D.h"

#include <ImGui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true) {}

// Layer on attach function
void Sandbox2D::OnAttach() 
{ 
	// Profiling
	PB_PROFILE_FUNCTION();

	// Load texture from file path
	m_CheckerboardTexture = PixelBrahma::Texture2D::Create("Assets/Textures/CheckerBoard.png");
}

// Layer on detach function
void Sandbox2D::OnDetach() 
{
	// Profiling
	PB_PROFILE_FUNCTION();
}

// Layer update function
void Sandbox2D::OnUpdate(PixelBrahma::Timestep timestep)
{
	// Sandbox update function profiling
	PB_PROFILE_FUNCTION();

	// Call camera update function
	m_CameraController.OnUpdate(timestep);

	{
		// Renderer preparation profiling
		PB_PROFILE_SCOPE("Renderer preparation");

		// Clear the color buffer
		PixelBrahma::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		PixelBrahma::RenderCommand::Clear();
	}

	{
		static float rotation = 0.0f;
		rotation += timestep * 50.0f;

		// Renderer drawing profiling
		PB_PROFILE_SCOPE("Renderer Draw");

		// Start rendering the 2D scene
		PixelBrahma::Renderer2D::BeginScene(m_CameraController.GetCamera());

		// Draw quads
		PixelBrahma::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
		PixelBrahma::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		PixelBrahma::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		PixelBrahma::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture, 10.0f);
		PixelBrahma::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_CheckerboardTexture, 20.0f);

		// Stop rendering the scene
		PixelBrahma::Renderer2D::EndScene();
	}
}

// ImGui render function
void Sandbox2D::OnImGuiRender()
{
	// Profiling
	PB_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

// Layer event handler function
void Sandbox2D::OnEvent(PixelBrahma::Event& event)
{
	m_CameraController.OnEvent(event);
}