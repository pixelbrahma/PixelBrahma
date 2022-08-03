#pragma once

#include <entt.hpp>

#include "PixelBrahma/Core/Timestep.h"

namespace PixelBrahma
{
	// Forward declaration of the entity class
	class Entity;

	// Scene management class
	class Scene
	{
	public:
		Scene();
		~Scene();

		// Create a new entity function
		Entity CreateEntity(const std::string& name = std::string());

		// Entity update function
		void OnUpdate(Timestep timestep);

	private:
		entt::registry m_Registry;

		friend class Entity;
	};
}