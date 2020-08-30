#ifndef CORE_H
#define CORE_H

/* @brief If the platform is Windows, and the project is the engines build,
		  then the dll should be exported. Othewise it is a different project
		  and thus the dll should be imported.
		  Else the platform is not Windows and thus the engine will not run.
*/
#ifdef UVK_PLATFORM_WINDOWS
	#ifdef UVK_BUILD_DLL
		#define UCINITI_API _declspec(dllexport)
	#else
		#define UCINITI_API _declspec(dllimport)
	#endif
#else
	#error Uciniti only supports Windows!
#endif // UVK_PLATFORM_WINDOWS

#ifdef UVK_ENABLE_ASSERTS
	#define UVK_CORE_ASSERT(x, ...) { if (!(x)) { UVK_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__);  __debugbreak(); } }
	#define UVK_ASSERT(x, ...) { if (!(x)) { UVK_ERROR("Assertion Failed: {0}", __VA_ARGS__);  __debugbreak(); } }
#else
	#define UVK_CORE_ASSERT(x, ...)
	#define UVK_ASSERT(x, ...)
#endif // UVK_ENABLE_ASSERTS

#define BIT(x) (1 << x)

#endif // !CORE_H