#include "pbpch.h"
#include "Renderer2D.h"

#include "Geometry/VertexArray.h"
#include "Shading/Shader.h"
#include "RenderCommand.h"

#include "glm/gtc/matrix_transform.hpp"

namespace PixelBrahma
{
	// Structure with renderer object data
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
		Ref<Shader> TextureShader;
	};

	static Renderer2DStorage* s_Data;

	// Initialize function
	void Renderer2D::Init()
	{
		// Create new renderer storage 
		s_Data = new Renderer2DStorage();
		
		// Set storage vertex array
		s_Data->QuadVertexArray = VertexArray::Create();

		// Vertices
		float squareVertices[5 * 4] = {
			// Positions            // Texture coordinates
			-0.5f, -0.5f, 0.0f,     0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,     1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f,     1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,     0.0f, 1.0f
		};

		// Vertex buffer
		Ref<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }

			});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

		// Indices
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

		// Set storage shaders
		s_Data->FlatColorShader = Shader::Create("Assets/Shaders/FlatColor.glsl");
		s_Data->TextureShader = Shader::Create("Assets/Shaders/Texture.glsl");
		
		// Bind and set uniform slot of the texture shader
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);
	}

	// Shutdown function for cleanup
	void Renderer2D::Shutdown() 
	{
		delete s_Data;
	}

	// Begin scene function
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		// Bind shaders and upload uniforms
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	// End Scene cleanup function
	void Renderer2D::EndScene() {}

	// Draw Quad functions

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		// Bind shaders and upload uniforms
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetFloat4("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		// Bind vertex array
		s_Data->QuadVertexArray->Bind();

		// Draw call
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		// Bind the texture and upload uniforms
		s_Data->TextureShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		texture->Bind();

		// Bind vertex array
		s_Data->QuadVertexArray->Bind();

		// Draw call
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
}