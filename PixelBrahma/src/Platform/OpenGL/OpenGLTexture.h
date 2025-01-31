#pragma once

#include "PixelBrahma/Renderer/Shading/Texture.h"

#include <glad/glad.h>

namespace PixelBrahma
{
	// Create and handle 2D texture in OpenGL
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		// Width and height getter functions override

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		// Get texture path override function
		virtual const std::string& GetPath() const override { return m_Path; }

		// Set texture data function override
		virtual void SetData(void* data, uint32_t size) override;

		// Texture bind function override
		virtual	void Bind(uint32_t slot = 0) const override;

		// Is texture loaded function override
		virtual bool IsLoaded() const override { return m_IsLoaded; }

		// Operator overloading for texture equality check - Right now checks using the renderer ID
		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == other.GetRendererID();
		}

	private:
		std::string m_Path;

		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;

		GLenum m_InternalFormat, m_DataFormat;

		bool m_IsLoaded = false;
	};
}
