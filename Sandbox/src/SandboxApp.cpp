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
		if (GE::Input::IsKeyPressed(GE::Key::A)) {
			GE_INFO("A is pressed");
		}
		// GE_INFO("ExampleLayer::Update");
	}

	void OnEvent(GE::Event& event) override
	{
		// GE_TRACE("{0}", e);
		if (event.GetEventType() == GE::EventType::KeyPressed)
		{
			GE::KeyPressedEvent& e = (GE::KeyPressedEvent&)event;
			GE_TRACE("{0}", (char)e.GetKeyCode());
		}
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
