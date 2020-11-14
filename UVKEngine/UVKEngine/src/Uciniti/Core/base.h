#ifndef BASE_H
#define BASE_H

#include <memory>

/* @brief If the platform is Windows, and the project is the engines build,
		  then the dll should be exported. Otherwise it is a different project
		  and thus the dll should be imported.
		  Else the platform is not Windows and thus the engine will not run.
*/
#ifdef UVK_PLATFORM_WINDOWS
	#if UVK_DYNAMIC_LINK
		#ifdef UVK_BUILD_DLL
			#define UCINITI_API _declspec(dllexport)
		#else
			#define UCINITI_API _declspec(dllimport)
		#endif // !UVK_BUILD_DLL
	#else
		#define UCINITI_API
	#endif // !UVK_DYNAMIC_LINK
#endif // !UVK_PLATFORM_WINDOWS

#ifdef UVK_DEBUG
	#define UVK_ENABLE_ASSERTS
#endif // !UVK_DEBUG

#ifdef UVK_ENABLE_ASSERTS
	#define UVK_CORE_ASSERT(x, ...) { if (!(x)) { UVK_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__);  __debugbreak(); } }
	#define UVK_ASSERT(x, ...) { if (!(x)) { UVK_ERROR("Assertion Failed: {0}", __VA_ARGS__);  __debugbreak(); } }
#else
	#define UVK_CORE_ASSERT(x, ...)
	#define UVK_ASSERT(x, ...)
#endif // UVK_ENABLE_ASSERTS

#define BIT(x) (1 << x)

// Pointer wrappers
namespace Uciniti
{
	template<typename T>
	using scope_ptr = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr scope_ptr<T> create_scope_ptr(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using ref_ptr = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr ref_ptr<T> create_ref_ptr(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	using byte = uint8_t;
}


#endif // !BASE_H