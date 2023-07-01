#include <GameEngine.h>
#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public GE::Layer
{
public:
	ExampleLayer()
		:Layer("example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), 
		m_CameraPosition(0.0f)
	{
		// plot the triangle
		/*	
		{
			// generate vertex array
			m_VertexArray.reset(GE::VertexArray::Create());

			// generate vertex buffer and buffer data
			float vertices[3 * 7] = { // default clip space: [-1 1]
				-0.4f, -0.4f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
				0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
				0.5f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
			};
			GE::Ref<GE::VertexBuffer> m_VertexBuffer;
			m_VertexBuffer.reset(GE::VertexBuffer::Create(vertices, sizeof(vertices)));

			GE::BufferLayout layout = {
				{GE::ShaderDataType::Float3, "a_Position" },
				{GE::ShaderDataType::Float4, "a_Color" }
			};
			m_VertexBuffer->SetLayout(layout);
			m_VertexArray->AddVertexBuffer(m_VertexBuffer);

			// generate index buffer
			uint32_t indices[3] = { 0, 1, 2 };
			GE::Ref<GE::IndexBuffer> m_IndexBuffer;
			m_IndexBuffer.reset(GE::IndexBuffer::Create(indices, 3));
			m_VertexArray->SetIndexBuffer(m_IndexBuffer);

			std::string vertexSrc = R"(
				#version 330 core
				layout(location = 0) in vec3 a_Position;
				layout(location = 1) in vec4 a_Color;
				uniform mat4 u_ViewProjection;
				uniform mat4 u_Transform;
				out vec4 v_Color;
				void main() {
					v_Color = a_Color;
					gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
				}
			)";
			std::string fragmentSrc = R"(
				#version 330 core
				layout(location = 0) out vec4 color;
				in vec4 v_Color;
				void main() {
					color = v_Color;
				}
			)";
			m_Shader.reset(GE::Shader::Create(vertexSrc, fragmentSrc));
		}
		*/


		// plot for grid
		// first create vertex array
		m_SquareVA = GE::VertexArray::Create();
		// create vertex buffer
		float SquareVertices[5 * 4] = { // default clip space: [-1 1]
		   -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		   -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		GE::Ref<GE::VertexBuffer> m_SquareVB;
		m_SquareVB = GE::VertexBuffer::Create(SquareVertices, sizeof(SquareVertices));
		// define layout of the data
		m_SquareVB->SetLayout({
			{GE::ShaderDataType::Float3, "a_Position" },
			{GE::ShaderDataType::Float2, "a_TexCoord" }
		});

		m_SquareVA->AddVertexBuffer(m_SquareVB);
		// create vertex index
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		GE::Ref<GE::IndexBuffer> m_SquareIB;
		m_SquareIB = GE::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVA->SetIndexBuffer(m_SquareIB);

		std::string vertexSrc2 = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			out vec3 v_Position;
			void main() {
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";
		std::string fragmentSrc2 = R"(
			#version 330 core
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			uniform vec4 u_Color;
			void main() {
				color = u_Color;
			}
		)";
		m_flatColorShader = GE::Shader::Create("shader1", vertexSrc2, fragmentSrc2);

		m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
		
		m_Texture = GE::Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoLogoTexture = GE::Texture2D::Create("assets/textures/ChernoLogo.png");
	}

	void OnUpdate(GE::Timestep ts) override
	{
		// GE_TRACE("Delta time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());
		{	// keyboard control
			if (GE::Input::IsKeyPressed(GE::Key::Left))
				m_CameraPosition.x += m_CameraMoveSpeed * ts;
			else if (GE::Input::IsKeyPressed(GE::Key::Right))
				m_CameraPosition.x -= m_CameraMoveSpeed * ts;
			if (GE::Input::IsKeyPressed(GE::Key::Down))
				m_CameraPosition.y += m_CameraMoveSpeed * ts;
			else if (GE::Input::IsKeyPressed(GE::Key::Up))
				m_CameraPosition.y -= m_CameraMoveSpeed * ts;

			if (GE::Input::IsKeyPressed(GE::Key::A))
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			else if (GE::Input::IsKeyPressed(GE::Key::D))
				m_CameraRotation += m_CameraRotationSpeed * ts;
		}
		GE::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		GE::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		GE::Renderer::BeginScene(m_Camera);

		// set scale and color
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		m_flatColorShader->Bind();
		std::dynamic_pointer_cast<GE::OpenGLShader>(m_flatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);
		
		// render a grid
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				glm::vec3 pos(i * 0.11, j * 0.11f, 0.0);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

				GE::Renderer::Submit(m_flatColorShader, m_SquareVA, transform);
			}
		}
		std::dynamic_pointer_cast<GE::OpenGLShader>(m_flatColorShader)->UploadUniformInt("u_Texture", 0);


		m_Texture->Bind();
		GE::Renderer::Submit(m_ShaderLibrary.Get("Texture"), m_SquareVA,
			glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.000001f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_ChernoLogoTexture->Bind();
		GE::Renderer::Submit(m_ShaderLibrary.Get("Texture"), m_SquareVA,
			glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));


		// triangle 
		// GE::Renderer::Submit(m_Shader, m_VertexArray);

		GE::Renderer::EndScene();
	}


	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(GE::Event& event) override
	{
		GE::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<GE::KeyPressedEvent>(GE_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(GE::KeyPressedEvent& event)
	{
		//if (event.GetKeyCode() == GE::Key::Left) 
		//	m_CameraPosition.x += m_CameraMoveSpeed;
		//else if (event.GetKeyCode() == GE::Key::Right)
		//	m_CameraPosition.x -= m_CameraMoveSpeed;
		//if (event.GetKeyCode() == GE::Key::Down)
		//	m_CameraPosition.y += m_CameraMoveSpeed;
		//else if (event.GetKeyCode() == GE::Key::Up)
		//	m_CameraPosition.y -= m_CameraMoveSpeed;

		return true;
	}

private:
	GE::ShaderLibrary m_ShaderLibrary;
	// plot for triangle
	GE::Ref<GE::Shader> m_Shader;
	GE::Ref<GE::VertexArray> m_VertexArray;

	// plot for flat color
	GE::Ref<GE::Shader> m_flatColorShader;
	GE::Ref<GE::VertexArray> m_SquareVA;

	GE::Ref<GE::Texture2D> m_Texture, m_ChernoLogoTexture;

	GE::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 0.3f;
	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 30.0f;

	glm::vec4 m_SquareColor = { 0.8f, 0.2f, 0.3f, 1.0f };
};

class Sandbox : public GE::Application
{
public:
	Sandbox() 
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {}
};

GE::Application* GE::CreateApplication() {
	return new Sandbox();
}
