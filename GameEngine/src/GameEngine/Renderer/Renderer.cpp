#include "gepch.h"
#include "GameEngine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
// #include "GameEngine/Renderer/Renderer2D.h"

namespace GE {

	// Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

	Renderer::SceneData *Renderer::s_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
		// Renderer2D::Init();
	}

	//void Renderer::Shutdown()
	//{
	//	Renderer2D::Shutdown();
	//}

	//void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	//{
	//	RenderCommand::SetViewport(0, 0, width, height);
	//}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}
