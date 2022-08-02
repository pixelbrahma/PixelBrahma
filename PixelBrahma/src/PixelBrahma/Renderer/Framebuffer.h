#pragma once

#include "PixelBrahma/Core/Core.h"

namespace PixelBrahma
{
	// structure of framebuffer properties
	struct FramebufferSpecification
	{
		uint32_t Width, Height;
		uint32_t Samples = 1;

		// Should or should not render to the screen
		bool SwapChainTarget = false;
	};

	// Framebuffer class
	class Framebuffer
	{
	public:

		// Bind and unbind functions

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		// Getter functions

		virtual uint32_t GetColorAttachmentRendererID() const = 0;
		virtual const FramebufferSpecification& GetSpecification() const = 0;

		// Create framebuffer function
		static Ref<Framebuffer> Create(const FramebufferSpecification& specification);
	};
}