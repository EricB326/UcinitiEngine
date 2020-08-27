workspace "UVKEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "UVKEngine"
	location "UVKEngine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. output_dir .. "/%{prj.name}")
	objdir ("bin-int/" .. output_dir .. "/%{prj.name}")

	pchheader "uvkpch.h"
	pchsource "UVKEngine/src/uvkpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"UVK_PLATFORM_WINDOWS",
			"UVK_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. output_dir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "UVK_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "UVK_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "UVK_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. output_dir .. "/%{prj.name}")
	objdir ("bin-int/" .. output_dir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"UVKEngine/vendor/spdlog/include",
		"UVKEngine/src"
	}

	links
	{
		"UVKEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"UVK_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "UVK_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "UVK_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "UVK_DIST"
		optimize "On"