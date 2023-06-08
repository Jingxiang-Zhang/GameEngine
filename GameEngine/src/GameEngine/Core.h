#pragma once

#ifdef GE_PLATFORM_WiNDOWS
	#ifdef GE_BUILD_DLL
		#define GE_API __declspec(dllexport)
	#else
		#define GE_API __declspec(dllimport)
	#endif
#else
#error Game Engine currently only support Sindows
#endif


#ifdef GE_DEBUG
	#define GE_ENABLE_ASSERTS
#endif

#ifdef GE_ENABLE_ASSERTS
#define GE_ASSERT(x, ...) { if(!(x)) { GE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
#define GE_CORE_ASSERT(x, ...) { if(!(x)) { GE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
#else
#define GE_ASSERT(...)
#define GE_CORE_ASSERT(...)
#endif

#define BIT(x) (1 << x)