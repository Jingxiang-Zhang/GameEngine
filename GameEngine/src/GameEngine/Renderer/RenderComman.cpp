#include "gepch.h"
#include "GameEngine/Renderer/RenderCommand.h"

namespace GE {

	// Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}