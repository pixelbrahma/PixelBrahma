#pragma once

#include "PixelBrahma/Core/Core.h"

#include <string>

namespace PixelBrahma
{
	// Texture loading and handling interface
	class Texture
	{
	public:
		virtual ~Texture() = default;

		// Width and height getter functions

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		// Set texture data
		virtual void SetData(void* data, uint32_t size) = 0;

		// Bind texture function 
		virtual	void Bind(uint32_t slot = 0) const = 0;

		// Is the texture loaded function
		virtual bool IsLoaded() const = 0;

		// Texture equality check operator overload
		virtual bool operator==(const Texture& other) const = 0;
	};

	// 2D texture creation and handling interface
	class Texture2D : public Texture
	{
	public:

		// Static functions to create textures

		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
		static Ref<Texture2D> Create(const std::string& path);
	};
}
