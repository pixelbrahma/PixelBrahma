#pragma once

#include <string>

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
		static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);
	};
}