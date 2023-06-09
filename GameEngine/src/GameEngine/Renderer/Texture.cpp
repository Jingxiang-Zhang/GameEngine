#include "gepch.h"
#include "GameEngine/Renderer/Texture.h"
#include "GameEngine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace GE {

	//Ref<Texture2D> Texture2D::Create(const TextureSpecification& specification)
	//{
	//	switch (Renderer::GetAPI())
	//	{
	//	case RendererAPI::API::None:    GE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	//	case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(specification);
	//	}

	//	GE_CORE_ASSERT(false, "Unknown RendererAPI!");
	//	return nullptr;
	//}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    GE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(path);
		}

		GE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
