#include "gepch.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace GE
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		// %T: Time stamp; 
		// %n: Logger's name; 
		// %v: The actual text to log;
		// %^, %$: The color range.
		spdlog::set_pattern("%^[%T] %n: %v%$"); 
		s_CoreLogger = spdlog::stdout_color_mt("GE");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}


