#pragma once

#include "PixelBrahma/Layers/Layer.h"

#include "PixelBrahma/Events/ApplicationEvent.h"
#include "PixelBrahma/Events/KeyEvent.h"
#include "PixelBrahma/Events/MouseEvent.h"

namespace PixelBrahma
{
	// Class for defining an ImGui UI layer
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		// Layer override functions

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& event) override;
//		virtual void OnImGuiRender() override;

		// ImGui layer begin and end functions - Begin and End ImGui rendering

		void Begin();
		void End();

		// Set Event blocking
		void BlockEvents(bool block) { m_BlockEvents = block; }

		// Set the theme to dark colors function
		void SetDarkThemeColors();

	private:
		bool m_BlockEvents = true;
	};
}
