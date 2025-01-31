#pragma once

#include "PixelBrahma/Core/Core.h"
#include "PixelBrahma/Core/Timestep.h"

#include "PixelBrahma/Events/Event.h"

namespace PixelBrahma
{
	// Class to hold layer data and manage event handling
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		// Virtual functions for layer management and event handling

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep timestep) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}

		// Get name of the layer
		const std::string& GetName() const { return m_DebugName; }

	private:
		std::string m_DebugName;
	};
}
