#include "pbpch.h"
#include "OpenGLVertexArray.h"

#include "glad/glad.h"

namespace PixelBrahma
{
	// Function to convert shader data type to OpenGL base date type
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			// Float data types
		case PixelBrahma::ShaderDataType::Float:  return GL_FLOAT;
		case PixelBrahma::ShaderDataType::Float2: return GL_FLOAT;
		case PixelBrahma::ShaderDataType::Float3: return GL_FLOAT;
		case PixelBrahma::ShaderDataType::Float4: return GL_FLOAT;

			// Matrix data types
		case PixelBrahma::ShaderDataType::Mat3:   return GL_FLOAT;
		case PixelBrahma::ShaderDataType::Mat4:   return GL_FLOAT;

			// Integer data types
		case PixelBrahma::ShaderDataType::Int:    return GL_INT;
		case PixelBrahma::ShaderDataType::Int2:   return GL_INT;
		case PixelBrahma::ShaderDataType::Int3:   return GL_INT;
		case PixelBrahma::ShaderDataType::Int4:   return GL_INT;

			// Boolean data types
		case PixelBrahma::ShaderDataType::Bool:   return GL_BOOL;
		}

		PB_CORE_ASSERT(false, "Unknown shader data type!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	// Vertex array bind and unbind functions

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}

	// Add vertex buffer
	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		PB_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout!");

		// Bind vertex array and buffer
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();

		for (const auto& element : layout)
		{
			// Set vertex array attributes pointer
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);

			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	// Set the Index buffer
	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		// Bind vertex array and index buffer
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}