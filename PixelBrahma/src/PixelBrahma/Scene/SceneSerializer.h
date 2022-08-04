#pragma once

#include "Scene.h"

namespace PixelBrahma
{
	// Class to serialize and deserialize the scene data
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		// Serialize functions 

		void Serialize(const std::string& filepath);
		void SerializeRuntime(const std::string& filepath);

		// Deserialize functions

		bool Deserialize(const std::string& filepath);
		bool DeserializeRuntime(const std::string& filepath);

	private:
		Ref<Scene> m_Scene;
	};
}
