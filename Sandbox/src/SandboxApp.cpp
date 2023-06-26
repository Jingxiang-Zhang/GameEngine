#include <GameEngine.h>
#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>


class ExampleLayer : public GE::Layer
{
public:
	ExampleLayer()
		:Layer("example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), 
		m_CameraPosition(0.0f)
	{
		// generate vertex array
		m_VertexArray.reset(GE::VertexArray::Create());

		// generate vertex buffer and buffer data
		float vertices[3 * 7] = { // default clip space: [-1 1]
			-0.4f, -0.4f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			0.5f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
		};
		std::shared_ptr<GE::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(GE::VertexBuffer::Create(vertices, sizeof(vertices)));

		GE::BufferLayout layout = {
			{GE::ShaderDataType::Float3, "a_Position" },
			{GE::ShaderDataType::Float4, "a_Color" }
		};
		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		// generate index buffer
		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<GE::IndexBuffer> m_IndexBuffer;
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
		m_Shader.reset(new GE::Shader(vertexSrc, fragmentSrc));




		// test for another vertex array
		// first create vertex array
		m_SquareVA.reset(GE::VertexArray::Create());
		// create vertex buffer
		float SquareVertices[3 * 4] = { // default clip space: [-1 1]
		   -0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f,  0.5f, 0.0f,
		   -0.5f,  0.5f, 0.0f
		};
		std::shared_ptr<GE::VertexBuffer> m_SquareVB;
		m_SquareVB.reset(GE::VertexBuffer::Create(SquareVertices, sizeof(SquareVertices)));
		// define layout of the data
		m_SquareVB->SetLayout({
			{
				GE::ShaderDataType::Float3, "a_Position" }
			});
		m_SquareVA->AddVertexBuffer(m_SquareVB);
		// create vertex index
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<GE::IndexBuffer> m_SquareIB;
		m_SquareIB.reset(GE::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
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
		m_Shader2.reset(new GE::Shader(vertexSrc2, fragmentSrc2));
	}

	void OnUpdate(GE::Timestep ts) override
	{
		// GE_TRACE("Delta time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

		if(GE::Input::IsKeyPressed(GE::Key::Left))
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

		GE::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		GE::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		GE::Renderer::BeginScene(m_Camera);
		
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.0f);
		glm::vec4 blueColor(0.2f, 0.3f, 0.8f, 1.0f);


		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				glm::vec3 pos(i * 0.11, j * 0.11f, 0.0);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				if (j % 2 == 0)
					m_Shader2->UploadUniformFloat4("u_Color", redColor);
				else
					m_Shader2->UploadUniformFloat4("u_Color", blueColor);
				GE::Renderer::Submit(m_Shader2, m_SquareVA, transform);
			}
		}
		GE::Renderer::Submit(m_Shader, m_VertexArray);

		GE::Renderer::EndScene();
	}



	virtual void OnImGuiRender() override
	{
		
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
	// test 1
	std::shared_ptr<GE::Shader> m_Shader;
	std::shared_ptr<GE::VertexArray> m_VertexArray;

	// test 2
	std::shared_ptr<GE::Shader> m_Shader2;
	std::shared_ptr<GE::VertexArray> m_SquareVA;

	GE::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 0.3f;
	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 30.0f;
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
