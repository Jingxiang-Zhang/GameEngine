#include "gepch.h"
#include "GameEngine/Renderer/RenderCommand.h"

namespace GE {

	// Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
	RendererAPI* RenderCommand::s_RendererAPI = RendererAPI::Create();

}