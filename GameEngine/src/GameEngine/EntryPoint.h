#pragma once
#ifdef GE_PLATFORM_WiNDOWS

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Log.h"


extern GE::Application* GE::CreateApplication();

int main(int argc, char** argv)
{
	GE::Log::Init();
	GE_CORE_WARN("Initialized Log!");
	GE_INFO("Hello! var = {0}", 5);

	GE::WindowResizeEvent e(1280, 720);
	GE_TRACE(e);

	GE::Application* app = GE::CreateApplication();
	app->Run();
	delete app;
}

#else
#error Game Engine currently only support Sindows
#endif

