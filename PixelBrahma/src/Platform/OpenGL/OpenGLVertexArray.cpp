#include "pbpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace PixelBrahma
{
	// Function to convert shader data type to OpenGL base date type
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			// Float data types
			case ShaderDataType::Float:  return GL_FLOAT;
			case ShaderDataType::Float2: return GL_FLOAT;
			case ShaderDataType::Float3: return GL_FLOAT;
			case ShaderDataType::Float4: return GL_FLOAT;

			// Matrix data types
			case ShaderDataType::Mat3:   return GL_FLOAT;
			case ShaderDataType::Mat4:   return GL_FLOAT;

			// Integer data types
			case ShaderDataType::Int:    return GL_INT;
			case ShaderDataType::Int2:   return GL_INT;
			case ShaderDataType::Int3:   return GL_INT;
			case ShaderDataType::Int4:   return GL_INT;

			// Boolean data types
			case ShaderDataType::Bool:   return GL_BOOL;
		}

		PB_CORE_ASSERT(false, "Unknown shader data type!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_RendererID);
	}

	// Vertex array bind and unbind functions

	void OpenGLVertexArray::Bind() const
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::UnBind() const
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	// Add vertex buffer
	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		PB_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout!");

		// Bind vertex array and buffer
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();

		for (const auto& element : layout)
		{
			// Set vertex array attributes pointer
			glEnableVertexAttribArray(m_VertexBufferIndex);
			glVertexAttribPointer(m_VertexBufferIndex,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);

			m_VertexBufferIndex++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	// Set the Index buffer
	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		// Bind vertex array and index buffer
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}