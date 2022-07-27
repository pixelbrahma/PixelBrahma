#include "pbpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

namespace PixelBrahma
{
	// Create default texture of given size
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) : m_Width(width), m_Height(height)
	{
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		// Create OpenGL texture
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		// Texture filtering and wrapping
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	// Load texture image from path
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_Path(path)
	{
		int width, height, channels;

		// Set vertical flipping of image to counter OpenGL image flip
		stbi_set_flip_vertically_on_load(1);

		// Load image data
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		PB_CORE_ASSERT(data, "Failed to load Image!");

		m_Width = width;
		m_Height = height;

		// Check number of texture channels for setting correct texture formats to support blending
		GLenum internalFormat = 0, dataFormat = 0;

		// RGBA format
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		// RGB format
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		PB_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		// Create OpenGL texture
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		// Texture filtering and wrapping
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Create texture sub image
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		// Free image
		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		// Delete OpenGL texture
		glDeleteTextures(1, &m_RendererID);
	}

	// Set texture data function
	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bytesPerChannel = m_DataFormat == GL_RGBA ? 4 : 3;

		PB_CORE_ASSERT(size == m_Width * m_Height * bytesPerChannel, "Data must be the entire texture!");

		// Create texture sub image
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	// Bind texture
	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}