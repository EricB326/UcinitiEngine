#ifndef CORE_H
#define CORE_H

#ifdef UVK_PLATFORM_WINDOWS
	#ifdef UVK_BUILD_DLL
		#define UCINITI_API _declspec(dllexport)
	#else
		#define UCINITI_API _declspec(dllimport)
	#endif
#else
	#error Uciniti only supports Windows!
#endif // UVK_PLATFORM_WINDOWS

#endif // !CORE_H