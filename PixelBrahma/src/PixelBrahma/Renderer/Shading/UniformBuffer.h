#pragma once

#include "PixelBrahma/Core/Core.h"

namespace PixelBrahma
{
	// Interface to hold uniform data in the GPU
	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() {}

		// Set uniform data function
		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;

		// Create buffer function
		static Ref<UniformBuffer> Create(uint32_t size, uint32_t binding);
	};
}
