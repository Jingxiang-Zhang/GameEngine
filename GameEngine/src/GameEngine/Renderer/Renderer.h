#pragma once
#include "GameEngine/Renderer/Shader.h"
#include "GameEngine/Renderer/RendererAPI.h"
#include "GameEngine/Renderer/RenderCommand.h"
#include "GameEngine/Renderer/OrthographicCamera.h"

namespace GE {

	class Renderer
	{
	public:
		// static void Init();
		// static void Shutdown();

		// static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();
		static void Submit(const std::shared_ptr<Shader>& shader, 
			const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};
		static SceneData* s_SceneData;

		// static Scope<SceneData> s_SceneData;
	};
}


