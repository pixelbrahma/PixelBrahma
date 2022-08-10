#include "pbpch.h"
#include "Shader.h"

#include "PixelBrahma/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace PixelBrahma
{
	//// Shader class ////

	// Create shader from source static function
	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			PB_CORE_ASSERT(false, "RendereAPI::None is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLShader>(filepath);
		}

		PB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	// Create shader from source static function
	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			PB_CORE_ASSERT(false, "RendereAPI::None is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		PB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	//// Shader library class ////

	// Add shader to the library with name
	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		PB_CORE_ASSERT(!Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	// Add shader to the library
	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	// Load shader from file path and add it to the library
	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	// Load Shader from file path, set name and add it to the library
	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	// Get shader by name from the library
	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		PB_CORE_ASSERT(Exists(name), "Shader not found!");
		return m_Shaders[name];
	}

	// Check if a shader exists in the library
	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}
