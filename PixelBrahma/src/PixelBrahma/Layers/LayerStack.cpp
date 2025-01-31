#include "pbpch.h"
#include "LayerStack.h"

namespace PixelBrahma
{
	LayerStack::~LayerStack()
	{
		// Delete all the layers in the stack
		for (Layer* layer : m_Layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	// Add a layer to the layer stack
	void LayerStack::PushLayer(Layer* layer)
	{
		// Insert layer at layer index and increment the index
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}

	// Add an overlay to the layer stack
	void LayerStack::PushOverlay(Layer* overlay)
	{
		// Add overlay add the back of the layer stack
		m_Layers.emplace_back(overlay);
	}

	// Remove a layer from the layer stack
	void LayerStack::PopLayer(Layer* layer)
	{
		// Find layer and remove it from the layer stack

		auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
		
		if (it != m_Layers.begin() + m_LayerInsertIndex)
		{
			layer->OnDetach();
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	// Remove an overlay from the layer stack
	void LayerStack::PopOverlay(Layer* overlay)
	{
		// Find overlay and remove it from the layer stack

		auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, overlay);

		if (it != m_Layers.end())
		{
			overlay->OnDetach();
			m_Layers.erase(it);
		}
	}
}