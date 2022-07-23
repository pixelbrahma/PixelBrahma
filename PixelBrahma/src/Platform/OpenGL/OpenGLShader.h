#pragma once

#include "PixelBrahma/Renderer/Shader.h"
#include "glm/glm.hpp"

// Temporary until OpenGL is abstracted from sandbox - Used for dynamic casting right now
typedef unsigned int GLenum;

namespace PixelBrahma
{
	// OpenGL shader class
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		// Bind and unbind functions override

		virtual void Bind() const override;
		virtual void UnBind() const override;

		// Get name of the shader
		virtual const std::string& GetName() const override { return m_Name; }

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
		
		// Function to process shaders from files and compile them

		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

	private:
		uint32_t m_RendererID;
		std::string m_Name;
	};
}