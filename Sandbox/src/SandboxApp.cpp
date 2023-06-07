#include <GameEngine.h>

class ExampleLayer : public GE::Layer
{
public:
	ExampleLayer()
		:Layer("example")
	{
	}

	void OnUpdate() override
	{
		// GE_INFO("ExampleLayer::Update");
	}

	void OnEvent(GE::Event& e) override
	{
		// GE_TRACE("{0}", e);
	}
};

class Sandbox : public GE::Application
{
public:
	Sandbox() 
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new GE::ImGuiLayer());
	}
	~Sandbox() {}
};

GE::Application* GE::CreateApplication() {
	return new Sandbox();
}
