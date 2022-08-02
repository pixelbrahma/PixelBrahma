#pragma once

#include "PixelBrahma/Renderer/Framebuffer.h"

namespace PixelBrahma
{
	// OpenGL framebuffer class
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& specification);
		virtual ~OpenGLFramebuffer();

		// Recreate the framebuffer function
		void Invalidate();

		// Bind and unbind override functions

		virtual void Bind() override;
		virtual void UnBind() override;

		// Resize override function
		virtual void Resize(uint32_t width, uint32_t height) override;

		// Getter functions override

		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }
		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;
		FramebufferSpecification m_Specification;
	};
}