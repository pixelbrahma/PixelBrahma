#pragma once

#include "PixelBrahma/Layers/Layer.h"

#include "PixelBrahma/Events/ApplicationEvent.h"
#include "PixelBrahma/Events/KeyEvent.h"
#include "PixelBrahma/Events/MouseEvent.h"

namespace PixelBrahma
{
	// Class for defining an ImGui UI layer
	class PIXELBRAHMA_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		// Layer override functions

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& e);

	private:

		// Mouse button events

		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);

		// Mouse move events

		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);

		// Key events

		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);

		// Window events

		bool OnWindowResizeEvent(WindowResizeEvent& e);

	private:
		float m_Time = 0.0f;
	};
}