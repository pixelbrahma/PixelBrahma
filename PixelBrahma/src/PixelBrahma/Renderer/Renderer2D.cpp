#include "pbpch.h"
#include "Renderer2D.h"

#include "Geometry/VertexArray.h"
#include "Shading/Shader.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace PixelBrahma
{
	// Structure with renderer object data
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
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
		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		// Vertex buffer
		Ref<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
			});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

		// Indices
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

		// Set storage shader
		s_Data->FlatColorShader = Shader::Create("Assets/Shaders/FlatColor.glsl");
	}

	// Shutdown function for cleanup
	void Renderer2D::Shutdown() 
	{
		delete s_Data;
	}

	// Begin scene function
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		// Bind shader and upload uniforms
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->
			UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->
			UploadUniformMat4("u_Transform", glm::mat4(1.0f));
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
		// Bind shader and upload uniforms
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformFloat4("u_Color", color);

		// Bind vertex array
		s_Data->QuadVertexArray->Bind();

		// Draw call
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
}