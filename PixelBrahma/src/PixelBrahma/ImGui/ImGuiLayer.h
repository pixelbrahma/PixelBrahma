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
		~ImGuiLayer() = default;

		// Layer override functions

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		// ImGui layer begin and end functions - Begin and End ImGui rendering

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};
}