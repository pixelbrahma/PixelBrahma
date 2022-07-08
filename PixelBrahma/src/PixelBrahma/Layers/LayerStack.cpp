#include "pbpch.h"
#include "LayerStack.h"

namespace PixelBrahma
{
	LayerStack::LayerStack()
	{
		// Set iterator pointer to the beginning of the stack
		m_LayerInsert = m_Layers.begin();
	}

	LayerStack::~LayerStack()
	{
		// Delete all the layers in the stack
		for (Layer* layer : m_Layers)
			delete layer;
	}

	// Add a layer to the layer stack
	void LayerStack::PushLayer(Layer* layer)
	{
		// Add layer at the iterator position
		m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
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

		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			m_LayerInsert--;
		}
	}

	// Remove an overlay from the layer stack
	void LayerStack::PopOverlay(Layer* overlay)
	{
		// Find overlay and remove it from the layer stack

		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);

		if (it != m_Layers.end())
			m_Layers.erase(it);
	}
}