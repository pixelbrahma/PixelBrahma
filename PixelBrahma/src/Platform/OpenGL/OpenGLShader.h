#pragma once

#include "PixelBrahma/Renderer/Shader.h"
#include "glm/glm.hpp"

namespace PixelBrahma
{
	// OpenGL shader class
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		// Bind and unbind functions override

		virtual void Bind() const override;
		virtual void UnBind() const override;

		//// Upload uniform functions ////

		// Integer uniforms

		void UploadUniformInt(const std::string& name, int value);

		// Float uniforms

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

		// Matrix uniforms

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		uint32_t m_RendererID;
	};
}