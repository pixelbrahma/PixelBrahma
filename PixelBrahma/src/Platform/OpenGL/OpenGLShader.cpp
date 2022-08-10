#include "pbpch.h"
#include "OpenGLShader.h"

#include "PixelBrahma/Core/Timer.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

#include <fstream>

namespace PixelBrahma
{
	namespace Utils
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

		// Convert opengl shader type to shaderc
		static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER:   return shaderc_glsl_vertex_shader;
				case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
			}

			PB_CORE_ASSERT(false);
			return (shaderc_shader_kind)0;
		}

		// Convert opengl shader type to string
		static const char* GLShaderStageToString(GLenum stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER:   return "GL_VERTEX_SHADER";
				case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
			}

			PB_CORE_ASSERT(false);
			return nullptr;
		}

		// Get the cache directory to build shader binaries to
		static const char* GetCacheDirectory()
		{
			return "Assets/Cache/Shader/OpenGL";
		}

		// Create cache directory if it does not exist
		static void CreateCacheDirectoryIfNeeded()
		{
			std::string cacheDirectory = GetCacheDirectory();

			if (!std::filesystem::exists(cacheDirectory))
				std::filesystem::create_directories(cacheDirectory);
		}

		// Extension of shader type binary files opengl
		static const char* GLShaderStageCachedOpenGLFileExtension(uint32_t stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER:    return ".cached_opengl.vert";
				case GL_FRAGMENT_SHADER:  return ".cached_opengl.frag";
			}

			PB_CORE_ASSERT(false);
			return "";
		}

		// Extension of shader type binary files vulkan
		static const char* GLShaderStageCachedVulkanFileExtension(uint32_t stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER:    return ".cached_vulkan.vert";
				case GL_FRAGMENT_SHADER:  return ".cached_vulkan.frag";
			}

			PB_CORE_ASSERT(false);
			return "";
		}
	}

	OpenGLShader::OpenGLShader(const std::string& filepath) : m_FilePath(filepath)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		// Create the cache directory if it does not exist
		Utils::CreateCacheDirectoryIfNeeded();

		// Read shader source file
		std::string source = ReadFile(filepath);

		// Split shader sources in the file
		auto shaderSources = PreProcess(source);

		// Compile individual shaders
		{
			Timer timer;

			// Compile API specific binaries for shaders 
			CompileOrGetVulkanBinaries(shaderSources);
			CompileOrGetOpenGLBinaries();

			// Create the shader program
			CreateProgram();
			
			PB_CORE_WARN("Shader creation took {0} ms", timer.ElapsedMillis());
		}

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

		// Compile API specific binaries for shaders 
		CompileOrGetVulkanBinaries(shaderSources);
		CompileOrGetOpenGLBinaries();

		// Create the shader program
		CreateProgram();
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
		std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII

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

			PB_CORE_ASSERT(Utils::ShaderTypeFromString(type), "Invalid shader type specified!");

			// Extract shader source
			// Start of shader code after shader type declaration line
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);

			PB_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax Error!");

			// Start of next shader type declaration line
			pos = source.find(typeToken, nextLinePos);
			shaderSources[Utils::ShaderTypeFromString(type)] = (pos == std::string::npos) ? 
				source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	// Compile shader to vulkan binary
	void OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		// Create program
		GLuint program = glCreateProgram();

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;

		// Set target vulkan environment and version
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		const bool optimize = true;

		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		// Directory to save the cache
		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

		auto& shaderData = m_VulkanSPIRV;
		shaderData.clear();

		for (auto&& [stage, source] : shaderSources)
		{
			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedPath = cacheDirectory /
				(shaderFilePath.filename().string() + Utils::GLShaderStageCachedVulkanFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);

			// Read from file if it exists
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				// Compile glsl shader to spirv
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source,
					Utils::GLShaderStageToShaderC(stage), m_FilePath.c_str(), options);

				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					PB_CORE_ERROR(module.GetErrorMessage());
					PB_CORE_ASSERT(false);
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				// Write spirv binary to cache file
				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);

				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}

		for (auto&& [stage, data] : shaderData)
			Reflect(stage, data);
	}

	// Compile shader to opengl binary
	void OpenGLShader::CompileOrGetOpenGLBinaries()
	{
		auto& shaderData = m_OpenGLSPIRV;

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;

		// Set opengl environment and version
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		const bool optimize = false;

		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		// Set directory to store the cache
		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

		shaderData.clear();
		m_OpenGLSourceCode.clear();

		for (auto&& [stage, spirv] : m_VulkanSPIRV)
		{
			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedPath = cacheDirectory / 
				(shaderFilePath.filename().string() + Utils::GLShaderStageCachedOpenGLFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);

			// Read from file if it exists
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				// Cross compile spriv code to glsl
				spirv_cross::CompilerGLSL glslCompiler(spirv);
				m_OpenGLSourceCode[stage] = glslCompiler.compile();
				auto& source = m_OpenGLSourceCode[stage];

				// Compile glsl code to spirv binary
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, 
					Utils::GLShaderStageToShaderC(stage), m_FilePath.c_str());

				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					PB_CORE_ERROR(module.GetErrorMessage());
					PB_CORE_ASSERT(false);
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				// Write spirv binary to cache file
				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);

				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}
	}

	// Create shader program
	void OpenGLShader::CreateProgram()
	{
		// Create program
		GLuint program = glCreateProgram();

		std::vector<GLuint> shaderIDs;

		for (auto&& [stage, spirv] : m_OpenGLSPIRV)
		{
			// Create and compile shader
			GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), 
				spirv.size() * sizeof(uint32_t));
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);

			// Shader compile status
			GLint isCompiled = 0;
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);

			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

				// Shader compile info log
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shaderID, maxLength, &maxLength, &infoLog[0]);

				// Display info log
				PB_CORE_ERROR("Shader compilation failed ({0}):\n{1}", m_FilePath, infoLog.data());

				// Delete the shader
				glDeleteShader(shaderID);
			}

			// Attach shader to program
			glAttachShader(program, shaderID);
		}

		// Link program
		glLinkProgram(program);

		// Program link status
		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);

		if (isLinked == GL_FALSE)
		{
			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// Program linking info log
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());

			// Display infolog
			PB_CORE_ERROR("Shader linking failed ({0}):\n{1}", m_FilePath, infoLog.data());

			// Delete program
			glDeleteProgram(program);

			// Delete the shaders
			for (auto id : shaderIDs)
				glDeleteShader(id);
		}

		for (auto id : shaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		m_RendererID = program;
	}

	// Display shader data
	void OpenGLShader::Reflect(GLenum stage, const std::vector<uint32_t>& shaderData)
	{
		spirv_cross::Compiler compiler(shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		PB_CORE_TRACE("OpenGLShader::Reflect - {0} {1}", Utils::GLShaderStageToString(stage), m_FilePath);
		PB_CORE_TRACE("    {0} uniform buffers", resources.uniform_buffers.size());
		PB_CORE_TRACE("    {0} resources", resources.sampled_images.size());

		PB_CORE_TRACE("Uniform buffers:");

		for (const auto& resource : resources.uniform_buffers)
		{
			const auto& bufferType = compiler.get_type(resource.base_type_id);
			uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			int memberCount = bufferType.member_types.size();

			PB_CORE_TRACE("  {0}", resource.name);
			PB_CORE_TRACE("    Size = {0}", bufferSize);
			PB_CORE_TRACE("    Binding = {0}", binding);
			PB_CORE_TRACE("    Members = {0}", memberCount);
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

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		PB_PROFILE_FUNCTION();

		UploadUniformFloat2(name, value);
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
