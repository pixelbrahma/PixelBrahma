#pragma once

#include "Entity.h"

namespace PixelBrahma
{
	// Class that defines an entity that can attach scripts
	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}

		// Get component template function
		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep timestep) {}

	private:
		Entity m_Entity;

		friend class Scene;
	};
}
