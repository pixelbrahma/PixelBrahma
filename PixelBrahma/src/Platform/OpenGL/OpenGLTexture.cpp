#include "pbpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

#include "glad/glad.h"

namespace PixelBrahma
{
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

		// Create OpenGL texture
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, GL_RGB8, m_Width, m_Height);

		// Texture filtering
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Create texture sub image
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, data);

		// Free image
		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		// Delete OpenGL texture
		glDeleteTextures(1, &m_RendererID);
	}

	// Bind texture
	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}