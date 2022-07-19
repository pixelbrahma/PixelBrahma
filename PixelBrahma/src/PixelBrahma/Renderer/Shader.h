#pragma once

#include <string>

#include "glm/glm.hpp"

namespace PixelBrahma
{
	// Class to handle shaders and use them
	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		// Shader bind and unbind functions

		void Bind() const;
		void UnBind() const;

		// Upload uniform matrices
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		uint32_t m_RendererID;
	};
}