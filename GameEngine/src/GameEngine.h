#pragma once

// For use by GameEngine Applications

// basic include
#include "GameEngine/Application.h"
#include "GameEngine/Layer.h"
#include "GameEngine/Log.h"
#include "GameEngine/ImGui/ImGuiLayer.h"
#include "GameEngine/Input.h"

#include "GameEngine/Core/Timestep.h"

// game engine key codes
#include "GameEngine/KeyCodes.h"
#include "GameEngine/MouseCodes.h"

// game engine event
#include "GameEngine/Events/Event.h"
#include "GameEngine/Events/ApplicationEvent.h"
#include "GameEngine/Events/KeyEvent.h"
#include "GameEngine/Events/MouseEvent.h"

// renderer
#include "GameEngine/Renderer/Renderer.h"
#include "GameEngine/Renderer/RenderCommand.h"
#include "GameEngine/Renderer/Buffer.h"
#include "GameEngine/Renderer/Shader.h"
#include "GameEngine/Renderer/VertexArray.h"
#include "GameEngine/Renderer/OrthographicCamera.h"

// Entry Point
#include "GameEngine/EntryPoint.h"
