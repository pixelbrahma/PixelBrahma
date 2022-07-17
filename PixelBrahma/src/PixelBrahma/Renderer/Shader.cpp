#include "pbpch.h"
#include "Shader.h"

#include "glad/glad.h"

namespace PixelBrahma
{
	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
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

	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);
	}

	// Bind shader function
	void Shader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	// Unbind shader function
	void Shader::UnBind() const
	{
		glUseProgram(0);
	}
}