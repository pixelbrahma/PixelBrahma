#pragma once

#include "PixelBrahma/Core/UUID.h"
#include "Scene.h"
#include "Components.h"

#include <entt.hpp>

namespace PixelBrahma
{
	// Class to handle entities and their operations
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		// Add component template function with variable arguments
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			PB_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			
			// Add the component to the entity
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			// Call the on component added function
			m_Scene->OnComponentAdded<T>(*this, component);

			return component;
		}

		// Component add or replace function
		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		// Get component template function
		template<typename T>
		T& GetComponent()
		{
			PB_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		// Has component template function
		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		// Remove component template function
		template<typename T>
		void RemoveComponent()
		{
			PB_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		// Operator overload to check if an entity handle is null
		operator bool() const { return m_EntityHandle != entt::null; }

		// Cast entt::entity type to entity handle type
		operator entt::entity() const { return m_EntityHandle; }

		// Operator overloading to convert ent::entity type to uint32_t
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }

		// Get UUID function
		UUID GetUUID() { return GetComponent<IDComponent>().ID; }

		// Get entity name function
		const std::string& GetName() { return GetComponent<TagComponent>().Tag; }

		// Operator overloading to check equality of entities
		bool operator==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}

		// Operatoe overloading to check not equal to 
		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}
