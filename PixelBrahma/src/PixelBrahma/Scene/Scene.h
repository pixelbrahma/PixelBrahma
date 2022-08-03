#pragma once

#include <entt.hpp>

#include "PixelBrahma/Core/Timestep.h"

namespace PixelBrahma
{
	// Scene management class
	class Scene
	{
	public:
		Scene();
		~Scene();

		// Create a new entity function
		entt::entity CreateEntity();

		// Get registry function
		entt::registry& GetRegistry() { return m_Registry; }

		// Entity update function
		void OnUpdate(Timestep timestep);

	private:
		entt::registry m_Registry;
	};
}