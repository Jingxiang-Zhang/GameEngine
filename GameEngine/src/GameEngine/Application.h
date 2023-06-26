#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "Events/ApplicationEvent.h"
#include "GameEngine/ImGui/ImGuiLayer.h"

#include "GameEngine/Core/Timestep.h"


namespace GE {

	class GE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		Timestep m_Timestep;
		float m_LastFrameTime;

	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}
