workspace "UVKEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "UVKEngine/vendor/GLFW/include"
IncludeDir["Vulkan"] = "UVKEngine/vendor/Vulkan/include"
IncludeDir["ImGui"] = "UVKEngine/vendor/ImGui"

LibraryDir = {}
LibraryDir["Vulkan"] = "vendor/Vulkan/lib/vulkan-1.lib"

include "UVKEngine/vendor/GLFW"
include "UVKEngine/vendor/ImGui"

project "UVKEngine"
	location "UVKEngine"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. output_dir .. "/%{prj.name}")
	objdir ("bin-int/" .. output_dir .. "/%{prj.name}")

	pchheader "uvkpch.h"
	pchsource "UVKEngine/src/uvkpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Vulkan}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"GLFW",
		"%{LibraryDir.Vulkan}",
		"ImGui"
	}

	filter "system:windows"
		cppdialect "C++17"
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
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "UVK_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "UVK_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"
	
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
		systemversion "latest"

		defines
		{
			"UVK_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "UVK_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "UVK_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "UVK_DIST"
		runtime "Release"
		optimize "On"