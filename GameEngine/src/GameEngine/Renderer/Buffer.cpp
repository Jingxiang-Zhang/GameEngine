#include "gepch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "GameEngine/Renderer/RendererAPI.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace GE
{
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			GE_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}
		GE_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			GE_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLIndexBuffer>(indices, count);
		}
		GE_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}


