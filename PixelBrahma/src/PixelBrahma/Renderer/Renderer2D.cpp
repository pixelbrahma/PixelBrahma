#include "pbpch.h"
#include "Renderer2D.h"

#include "Geometry/VertexArray.h"
#include "Shading/Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace PixelBrahma
{
	// Quad vertex data structure
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
	};

	// Structure with renderer object data
	struct Renderer2DData
	{
		// Batch Rendering batch size

		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
		
		// Vertex buffer batching variables

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		// Textures batching variables

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;		// Because slot 0 is the dummy texture

		glm::vec4 QuadVertexPositions[4];

		// Stats
		Renderer2D::Statistics Stats;
	};

	static Renderer2DData s_Data;

	// Initialize function
	void Renderer2D::Init()
	{
		// Profiling
		PB_PROFILE_FUNCTION();
		
		// Set storage vertex array
		s_Data.QuadVertexArray = VertexArray::Create();

		// Create vertex buffer, set layout and add it to the vertex array
		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"     },
			{ ShaderDataType::Float4, "a_Color"        },
			{ ShaderDataType::Float2, "a_TexCoord"     },
			{ ShaderDataType::Float,  "a_TexIndex"     },
			{ ShaderDataType::Float,  "a_TilingFactor" }
			});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		// Array of vertex buffer vertices
		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		// Array of indices
		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;

		// Set index offsets
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}
		
		// Index buffer
		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		// Create default texture
		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		// Texture samplers for each texture
		int32_t samplers[s_Data.MaxTextureSlots];

		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		// Create and bind shader from file path
		s_Data.TextureShader = Shader::Create("Assets/Shaders/Texture.glsl");
		
		// Bind and set uniform slot of the texture shader
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		// Set quad vertex positions
		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	// Shutdown function for cleanup
	void Renderer2D::Shutdown() 
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		delete[] s_Data.QuadVertexBufferBase;
	}

	// Begin scene function
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	// End Scene cleanup function
	void Renderer2D::EndScene() 
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		// Set the pointer to the end of the batch and flush the buffer 

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	// Flush buffers
	void Renderer2D::Flush()
	{
		// Render nothing if there is nothing to draw
		if (s_Data.QuadIndexCount == 0)
			return;

		// Bind all the textures in the texture slots
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		// Draw call for batched quads
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
		s_Data.Stats.DrawCalls++;

	}

	// Flush and end rendering
	void Renderer2D::FlushAndReset()
	{
		// End the scene
		EndScene();

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	// Draw Quad functions

	// Draw quad

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		const float textureIndex = 0.0f;		// Dummy texture
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float tilingFactor = 1.0f;

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		// Transformation matrix
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		// Set quad vertex buffer pointer attributes for each of the quad vertices

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position        = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color           = color;
			s_Data.QuadVertexBufferPtr->TexCoord        = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex        = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor    = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,
		float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, 
		float tilingFactor, const glm::vec4& tintColor)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		float textureIndex = 0.0f;

		// Find and set index of the texture
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				FlushAndReset();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		// Transformation matrix
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		// Set quad vertex buffer pointer attributes for each of the quad vertices

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position        = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color           = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord        = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex        = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor    = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.Stats.QuadCount++;
	}

	// Draw rotated quad

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, 
		const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, 
		const glm::vec4& color)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		const float textureIndex = 0.0f;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float tilingFactor = 1.0f;

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		// Set transformation matrix
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.TextureShader->SetMat4("u_Transform", transform);

		// Set quad vertex buffer pointer attributes for each of the quad vertices

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position        = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color           = color;
			s_Data.QuadVertexBufferPtr->TexCoord        = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex        = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor    = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, 
		const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, 
		const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			FlushAndReset();

		float textureIndex = 0.0f;

		// Find and set index of the texture
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				FlushAndReset();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		// Set transformation matrix
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		// Set quad vertex buffer pointer attributes for each of the quad vertices

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position        = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color           = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord        = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex        = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor    = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}
		
		s_Data.QuadIndexCount += 6;
		s_Data.Stats.QuadCount++;
	}

	// Reset stats
	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	// Get stats
	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}
}