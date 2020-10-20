project "shaderc"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	targetdir ("bin/" .. output_dir .. "/%{prj.name}")
	objdir ("bin-int/" .. output_dir .. "/%{prj.name}")

	files
	{
		"include/shaderc/env.h",
		"include/shaderc/shaderc.h"
		"include/shaderc/shaderc.hpp"
		"include/shaderc/status.h"
		"include/shaderc/visibility.h"
	}
	
	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"