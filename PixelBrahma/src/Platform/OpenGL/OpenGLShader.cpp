#include "pbpch.h"
#include "OpenGLShader.h"

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

namespace PixelBrahma
{
	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		//// Vertex Shader ////

		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL

		const GLchar* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		// Check if vertex shader compilation is sucessful, otherwise print error

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);

		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;

			// Get error log length
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);

			// Get the error message
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// Delete the shader
			glDeleteShader(vertexShader);

			// Print error
			PB_CORE_ERROR("{0}", infoLog.data());
			PB_CORE_ASSERT(false, "Vertex shader compilation failure!");

			return;
		}

		//// Fragment Shader ////

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL

		source = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		// Check if fragment shader compilation is sucessful, otherwise print error

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);

		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;

			// Get error log length
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);

			// Get the error message
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// Delete the shaders
			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);

			// Print error
			PB_CORE_ERROR("{0}", infoLog.data());
			PB_CORE_ASSERT(false, "Fragment shader compilation failure!");

			return;
		}

		//// Shader program ////

		// After successful shader compilation, link them together in a program
		m_RendererID = glCreateProgram();
		GLuint program = m_RendererID;

		// Attach shaders to the program
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		// Link the program
		glLinkProgram(program);

		// Check if program linking is sucessful, otherwise print error

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);

		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;

			// Get error log length
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);

			// Get the error message
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// Delete the program and shaders
			glDeleteProgram(program);
			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);

			// Print error
			PB_CORE_ERROR("{0}", infoLog.data());
			PB_CORE_ASSERT(false, "Shader link failure!");

			return;
		}

		// Detach shaders after successful compilation
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	// Bind the shader and use shader program
	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	// Unbind the shader
	void OpenGLShader::UnBind() const
	{
		glUseProgram(0);
	}

	//// Upload uniform funcions ////

	// Integer uniforms

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		// Get uniform location from name and set the value
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	// Float uniforms

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		// Get uniform location from name and set the value
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		// Get uniform location from name and set the value
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		// Get uniform location from name and set the value
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		// Get uniform location from name and set the value
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	// Matrix uniforms

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		// Get uniform location from name and set the value
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		// Get location of the uniform in the shader and set the uniform matrix

		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}