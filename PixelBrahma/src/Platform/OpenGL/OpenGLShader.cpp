#include "pbpch.h"
#include "OpenGLShader.h"

#include <fstream>

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace PixelBrahma
{
	// Function to get shader type from the string
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		else if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		PB_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		// Read shader source file
		std::string source = ReadFile(filepath);

		// Split shader sources in the file
		auto shaderSources = PreProcess(source);

		// Compile individual shaders
		Compile(shaderSources);

		// Extract the name from filepath

		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, 
		const std::string& vertexSrc, const std::string& fragmentSrc) : m_Name(name)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		// Set each shader sources to the corresponding shader type key in the map
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexSrc;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;

		// Compile the individual shaders
		Compile(shaderSources);
	}

	OpenGLShader::~OpenGLShader()
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

	// Read string from shader file
	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		std::string result;

		// Open input binary file stream 
		std::ifstream in(filepath, std::ios::in | std::ios::binary);

		if (in)
		{
			// Set cursor position to the end of the file
			in.seekg(0, std::ios::end);

			// Get the size of the file to the position of the cursor(number of characters)
			size_t size = in.tellg();

			if (size != -1)
			{
				// Resize the file to the position of the cursor(number of characters) 
				result.resize(size);

				// Set the cursor position to the beginning of the file, read file stream and close the file
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
				in.close();
			}
			else
				PB_CORE_ERROR("Could not read from file '{0}'", filepath);
		}
		else
			PB_CORE_ERROR("Could not open file '{0}'", filepath);

		return result;
	}

	// Split shader source into separate shader types and map them correspondingly
	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;

		// Token marking the beginning of a shader source
		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);

		// Find type token in source - Start of shader type declaration line
		size_t pos = source.find(typeToken, 0);

		while (pos != std::string::npos)
		{
			// Find end of line - End of shader type declaration line
			size_t eol = source.find_first_of("\r\n", pos);
			PB_CORE_ASSERT(eol != std::string::npos, "Syntax error!");

			// Extract shader type and assert as a supported type - Start of shader type name
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			PB_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified!");

			// Extract shader source
			// Start of shader code after shader type declaration line
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			PB_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax Error!");

			// Start of next shader type declaration line
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? 
				source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	// Compile shaders function
	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		// Create program
		GLuint program = glCreateProgram();
	
		PB_CORE_ASSERT(shaderSources.size() <= 2, "Only two shaders are supported!");

		// Create an array of shader ID's
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIndex = 0;

		//// Shaders compilation ////

		for (auto& kv : shaderSources)
		{
			// Shader type
			GLenum type = kv.first;

			// Shader source string
			const std::string& source = kv.second;

			// Create shader of the shader type
			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			
			// Assign shader source of the shader type
			glShaderSource(shader, 1, &sourceCStr, 0);

			// Compile shader
			glCompileShader(shader);

			// Check if vertex shader compilation is sucessful, otherwise print error

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;

				// Get error log length
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);

				// Get the error message
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// Delete the shader
				glDeleteShader(shader);

				// Print error
				PB_CORE_ERROR("{0}", infoLog.data());
				PB_CORE_ASSERT(false, "Shader compilation failure!");

				break;
			}

			// Attach shader to the program and add shader ID's for cleanup
			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		//// Program Linking ////

		// After successful shader compilation, link them together in a program
		m_RendererID = program;

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
			
			for (auto id : glShaderIDs)
				glDeleteShader(id);

			// Print error
			PB_CORE_ERROR("{0}", infoLog.data());
			PB_CORE_ASSERT(false, "Shader link failure!");

			return;
		}

		// Detach shaders after successful compilation
		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}
	}

	// Bind the shader and use shader program
	void OpenGLShader::Bind() const
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
	}

	// Unbind the shader
	void OpenGLShader::UnBind() const
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	// Set uniform functions 

	// Integers

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		UploadUniformIntArray(name, values, count);
	}

	// Floats

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		UploadUniformFloat4(name, value);
	}

	// Matrices

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		UploadUniformMat4(name, value);
	}

	//// Upload uniform funcions ////

	// Integer uniforms

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		// Get uniform location from name and set the value
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
	{
		// Get uniform location from name and set the value
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, values);
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