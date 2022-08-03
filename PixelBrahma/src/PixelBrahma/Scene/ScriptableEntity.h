#pragma once

#include "Entity.h"

namespace PixelBrahma
{
	// Class that defines an entity that can attach scripts
	class ScriptableEntity
	{
	public:

		// Get component template function
		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	private:
		Entity m_Entity;

		friend class Scene;
	};
}