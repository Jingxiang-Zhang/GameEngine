#pragma once
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#pragma warning(push, 0)

namespace GE 
{
	class GE_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}
#pragma warning(pop)

// Core log macros
#define GE_CORE_TRACE(...)    ::GE::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define GE_CORE_INFO(...)     ::GE::Log::GetCoreLogger()->info(__VA_ARGS__)
#define GE_CORE_WARN(...)     ::GE::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define GE_CORE_ERROR(...)    ::GE::Log::GetCoreLogger()->error(__VA_ARGS__)
#define GE_CORE_CRITICAL(...) ::GE::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define GE_TRACE(...)         ::GE::Log::GetClientLogger()->trace(__VA_ARGS__)
#define GE_INFO(...)          ::GE::Log::GetClientLogger()->info(__VA_ARGS__)
#define GE_WARN(...)          ::GE::Log::GetClientLogger()->warn(__VA_ARGS__)
#define GE_ERROR(...)         ::GE::Log::GetClientLogger()->error(__VA_ARGS__)
#define GE_CRITICAL(...)      ::GE::Log::GetClientLogger()->critical(__VA_ARGS__)