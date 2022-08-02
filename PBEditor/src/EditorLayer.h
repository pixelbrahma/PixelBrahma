#pragma once

#include <PixelBrahma.h>

namespace PixelBrahma
{
	// 2D application layer class
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		// Layer attach and detach overrides

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		// Update function override
		virtual void OnUpdate(Timestep timestep) override;

		// ImGui render function override
		virtual void OnImGuiRender() override;

		// Event handling function override
		virtual void OnEvent(Event& event) override;

	private:
		OrthographicCameraController m_CameraController;

		// Temporary tests
		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColorShader;
		Ref<Framebuffer> m_Framebuffer;
		Ref<Texture2D> m_CheckerboardTexture;

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	};
}