#pragma once

#include <PixelBrahma.h>

// Example of layer creation
class ExampleLayer : public PixelBrahma::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	// Layer attach and detach functions

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	// Update and event handling functions

	void OnUpdate(PixelBrahma::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(PixelBrahma::Event& e) override;

private:
	// Shader library
	PixelBrahma::ShaderLibrary m_ShaderLibrary;

	// Triangle object
	PixelBrahma::Ref<PixelBrahma::Shader> m_Shader;
	PixelBrahma::Ref<PixelBrahma::VertexArray> m_VertexArray;

	// Square object
	PixelBrahma::Ref<PixelBrahma::Shader> m_FlatColorShader;
	PixelBrahma::Ref<PixelBrahma::VertexArray> m_SquareVA;

	// Checkerboard texture
	PixelBrahma::Ref<PixelBrahma::Texture2D> m_Texture;

	// Logo
	PixelBrahma::Ref<PixelBrahma::Texture2D> m_LogoTexture;

	// Camera controller object
	PixelBrahma::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};