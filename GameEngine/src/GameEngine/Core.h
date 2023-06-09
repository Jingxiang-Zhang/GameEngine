#pragma once
#include <memory>

#ifdef GE_PLATFORM_WiNDOWS
	#ifdef GE_DYNAMIC_LINK
		#ifdef GE_BUILD_DLL
			#define GE_API __declspec(dllexport)
		#else
			#define GE_API __declspec(dllimport)
		#endif
	#else
		#define GE_API
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

#define GE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace GE {

	// "using" keyword is a new version of typedef, and it can use template
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args && ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}