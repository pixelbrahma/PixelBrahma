#pragma once

#include "PixelBrahma.h"

// 2D application layer class
class Sandbox2D : public PixelBrahma::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	// Layer attach and detach overrides

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	// Update function override
	virtual void OnUpdate(PixelBrahma::Timestep timestep) override;

	// ImGui render function override
	virtual void OnImGuiRender() override;

	// Event handling function override
	virtual void OnEvent(PixelBrahma::Event& event) override;

private:
	PixelBrahma::OrthographicCameraController m_CameraController;

	// Temporary tests
	PixelBrahma::Ref<PixelBrahma::VertexArray> m_SquareVA;
	PixelBrahma::Ref<PixelBrahma::Shader> m_FlatColorShader;
	PixelBrahma::Ref<PixelBrahma::Framebuffer> m_Framebuffer;
	PixelBrahma::Ref<PixelBrahma::Texture2D> m_CheckerboardTexture;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};