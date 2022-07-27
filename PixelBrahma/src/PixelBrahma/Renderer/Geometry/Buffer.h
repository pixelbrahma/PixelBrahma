#pragma once

namespace PixelBrahma
{

	//// Shader data type properties and functions ////

	// Enumeration for shader data types
	enum class ShaderDataType
	{
		None = 0,

		// Float data types
		Float, Float2, Float3, Float4,

		// Matrix data types
		Mat3, Mat4,

		// Integer data types
		Int, Int2, Int3, Int4,

		// Boolean data type
		Bool
	};

	// Size of function for the shader data types
	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			// Float data types
			case ShaderDataType::Float:  return 4;
			case ShaderDataType::Float2: return 4 * 2;
			case ShaderDataType::Float3: return 4 * 3;
			case ShaderDataType::Float4: return 4 * 4;

			// Matrix data types
			case ShaderDataType::Mat3:   return 4 * 3 * 3;
			case ShaderDataType::Mat4:   return 4 * 4 * 4;

			// Integer data types
			case ShaderDataType::Int:    return 4;
			case ShaderDataType::Int2:   return 4 * 2;
			case ShaderDataType::Int3:   return 4 * 3;
			case ShaderDataType::Int4:   return 4 * 4;

			// Boolean data types
			case ShaderDataType::Bool:   return 1;
		}

		PB_CORE_ASSERT(false, "Unknown shader data type!");
		return 0;
	}

	// Structure describing the elements of the buffer
	struct BufferElement
	{
		// Element properties

		std::string	Name;
		ShaderDataType Type;
		uint32_t Size;
		size_t Offset;
		bool Normalized;

		BufferElement() = default;
		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false) :
			Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {}

		// Function to return number of elements in the data type
		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				// Float data types
				case ShaderDataType::Float:  return 1;
				case ShaderDataType::Float2: return 2;
				case ShaderDataType::Float3: return 3;
				case ShaderDataType::Float4: return 4;

				// Matrix data types
				case ShaderDataType::Mat3:   return 3 * 3;
				case ShaderDataType::Mat4:   return 4 * 4;

				// Integer data types
				case ShaderDataType::Int:    return 1;
				case ShaderDataType::Int2:   return 2;
				case ShaderDataType::Int3:   return 3;
				case ShaderDataType::Int4:   return 4;

				// Boolean data types
				case ShaderDataType::Bool:   return 1;
			}

			PB_CORE_ASSERT(false, "Unknown shader data type!");
			return 0;
		}
	};

	//// Buffer Layout ////

	// Vertex buffer layout class
	class BufferLayout
	{
	public:
		BufferLayout() {}
		BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements)
		{
			CalculateOffsetAndStride();
		}

		// Getter functions

		inline uint32_t GetStride() const { return m_Stride; }
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		// Iterator functions 

		std::vector<BufferElement>::iterator begin()             { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end()               { return m_Elements.end();   }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const   { return m_Elements.end();   }

	private:

		// Function to calculate offset and stride of the buffer layout
		void CalculateOffsetAndStride()
		{
			size_t offset = 0;
			m_Stride = 0;

			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}

	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	//// Vertex Buffer ////

	// Vertex buffer interface
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		// Vertex buffer functions

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		// Vertex buffer layout functions

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
	};

	//// Index Buffer ////

	// Index buffer interface
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	};
}