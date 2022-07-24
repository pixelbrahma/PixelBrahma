#pragma once

#include <string>
#include <unordered_map>

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

		// Create shader from source static function
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