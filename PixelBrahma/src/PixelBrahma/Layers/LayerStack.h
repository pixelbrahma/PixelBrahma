#pragma once

#include "PixelBrahma/Core/Core.h"
#include "Layer.h"

#include <vector>

namespace PixelBrahma
{
	// Wrapper class to maintain a vector of layers
	class PIXELBRAHMA_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		// Functions to add layers and overlays to the layer stack

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		// Functions to remove layers and overlays from the layer stack

		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		// Layer stack iterator functions

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<Layer*> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};
}