#pragma once

#include "PixelBrahma/Layers/Layer.h"

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
		float m_Time = 0.0f;
	};
}