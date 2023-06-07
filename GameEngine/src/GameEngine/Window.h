#pragma once

#include "GameEngine/Core.h"
#include "GameEngine/Events/Event.h"

namespace GE {

	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Game Engine",
			        uint32_t width = 1600,
			        uint32_t height = 900)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	// Interface representing a desktop system based Window
	class GE_API Window
	{
	public:
		// Introduced in C++11, using is the modern syntax for type aliases.
		// also equal to typedef std::function<void(Event&)> EventCallbackFn;
		// but "using" is more flexible and can be used with template parameters.
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};

}
