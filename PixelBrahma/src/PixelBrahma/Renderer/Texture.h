#pragma once

#include <string>

#include "PixelBrahma/Core.h"

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

		// Bind texture function 
		virtual	void Bind(uint32_t slot = 0) const = 0;
	};

	// 2D texture creation and handling interface
	class Texture2D : public Texture
	{
	public:

		// Static function to create texture from path
		static Ref<Texture2D> Create(const std::string& path);
	};
}