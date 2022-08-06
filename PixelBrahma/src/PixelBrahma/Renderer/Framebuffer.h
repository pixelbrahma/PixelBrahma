#pragma once

#include "PixelBrahma/Core/Core.h"

namespace PixelBrahma
{
	// Framebuffer texture format enum class
	enum class FramebufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,

		// Entity ID
		RED_INTEGER,

		// Depth/stencil
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8
	};

	// Framebuffer texture specification structure
	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
		// TODO: filtering/wrap
	};

	// Framebuffer attachment specification structure
	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	// structure of framebuffer properties
	struct FramebufferSpecification
	{
		uint32_t Width = 0;
		uint32_t Height = 0;
		uint32_t Samples = 1;

		FramebufferAttachmentSpecification Attachments;

		// Should or should not render to the screen
		bool SwapChainTarget = false;
	};

	// Framebuffer class
	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		// Bind and unbind functions

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		// Resize function
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		// Read Pixels from attachment function
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

		// Getter functions

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
		virtual const FramebufferSpecification& GetSpecification() const = 0;

		// Create framebuffer function
		static Ref<Framebuffer> Create(const FramebufferSpecification& specification);
	};
}
