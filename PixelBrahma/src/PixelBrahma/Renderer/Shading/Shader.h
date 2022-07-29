#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace PixelBrahma
{
	// Interface to handle shaders and use them
	class Shader
	{
	public:
		virtual ~Shader() = default;

		// Shader bind and unbind functions

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		// Set uniform functions

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

		// Create shader from source static functions

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, 
			const std::string& vertexSrc, const std::string& fragmentSrc);

		// Get shader name function
		virtual const std::string& GetName() const = 0;
	};

	// Shader library class to hold a map of shaders and manage them
	class ShaderLibrary
	{
	public:

		// Add Shader to the library functions

		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);

		// Load Shader from file functions

		Ref<Shader> Load(const std::string& name, const std::string& filepath);
		Ref<Shader> Load(const std::string& filepath);

		// Get shader function
		Ref<Shader> Get(const std::string& name);

		// Check if shader already exists in the library
		bool Exists(const std::string& name) const;

	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}