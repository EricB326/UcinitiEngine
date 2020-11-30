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
IncludeDir["ImGui"] = "UVKEngine/vendor/ImGui"
IncludeDir["glm"] = "UVKEngine/vendor/glm"
IncludeDir["shaderc"] = "UVKEngine/vendor/shaderc/include"
IncludeDir["SPIRV_Cross"] = "UVKEngine/vendor/SPIRV-Cross"
IncludeDir["Vulkan"] = "UVKEngine/vendor/Vulkan/include"
IncludeDir["stb_image"] = "UVKEngine/vendor/stb_image"
IncludeDir["assimp"] = "UVKEngine/vendor/assimp/include"

LibraryDir = {}
LibraryDir["shaderc"] = "vendor/shaderc/lib/shaderc.lib"
LibraryDir["shaderc_util"] = "vendor/shaderc/lib/shaderc_util.lib"
LibraryDir["glslang"] = "vendor/glslang/lib/debug/glslangd.lib"
LibraryDir["glslang_MachineIndependent"] = "vendor/glslang/lib/debug/MachineIndependentd.lib"
LibraryDir["glslang_SPIRV"] = "vendor/glslang/lib/debug/SPIRVd.lib"
LibraryDir["glslang_OGLCompiler"] = "vendor/glslang/lib/debug/OGLCompilerd.lib"
LibraryDir["glslang_OSDependent"] = "vendor/glslang/lib/debug/OSDependentd.lib"
LibraryDir["glslang_GenericCodeGen"] = "vendor/glslang/lib/debug/GenericCodeGend.lib"
LibraryDir["SPIRV_Tools"] = "vendor/SPIRV-Tools/lib/debug/SPIRV-Tools.lib"
LibraryDir["SPIRV_Tools_opt"] = "vendor/SPIRV-Tools/lib/debug/SPIRV-Tools-opt.lib"
LibraryDir["Vulkan"] = "vendor/Vulkan/lib/vulkan-1.lib"

include "UVKEngine/vendor/GLFW"
include "UVKEngine/vendor/ImGui"
include "UVKEngine/vendor/SPIRV-Cross"

project "UVKEngine"
	location "UVKEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"


	targetdir ("bin/" .. output_dir .. "/%{prj.name}")
	objdir ("bin-int/" .. output_dir .. "/%{prj.name}")

	pchheader "uvkpch.h"
	pchsource "UVKEngine/src/uvkpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vender/glm/glm/**.hpp",
		"%{prj.name}/vender/glm/glm/**.inl"
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
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.shaderc}",
		"%{IncludeDir.SPIRV_Cross}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.assimp}"
	}

	links
	{
		"GLFW",
		"ImGui",
		"SPIRV_Cross",
		"%{LibraryDir.Vulkan}",
		
		"%{LibraryDir.shaderc}",
		"%{LibraryDir.shaderc_util}",
		"%{LibraryDir.glslang}",
		"%{LibraryDir.glslang_MachineIndependent}",
		"%{LibraryDir.glslang_SPIRV}",
		"%{LibraryDir.glslang_OGLCompiler}",
		"%{LibraryDir.glslang_OSDependent}",
		"%{LibraryDir.glslang_GenericCodeGen}",
		"%{LibraryDir.SPIRV_Tools}",
		"%{LibraryDir.SPIRV_Tools_opt}"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"UVK_PLATFORM_WINDOWS",
			"UVK_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "UVK_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "UVK_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "UVK_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
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
		"UVKEngine/src",
		"UVKEngine/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"UVKEngine"
	}
	
	postbuildcommands
	{
		'{COPY} "../Sandbox/assets" "%{cfg.targetdir}/assets"'
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"UVK_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "UVK_DEBUG"
		runtime "Debug"
		symbols "on"

		links
		{
			"UVKEngine/vendor/assimp/bin/Debug/assimp-vc141-mtd.lib"
		}

		postbuildcommands 
		{
			'{COPY} "../UVKEngine/vendor/assimp/bin/Debug/assimp-vc141-mtd.dll" "%{cfg.targetdir}"'
		}

	filter "configurations:Release"
		defines "UVK_RELEASE"
		runtime "Release"
		optimize "on"

		links
		{
			"UVKEngine/vendor/assimp/bin/Release/assimp-vc141-mt.lib"
		}

		postbuildcommands 
		{
			'{COPY} "../UVKEngine/vendor/assimp/bin/Release/assimp-vc141-mt.dll" "%{cfg.targetdir}"'
		}

	filter "configurations:Dist"
		defines "UVK_DIST"
		runtime "Release"
		optimize "on"

		links
		{
			"UVKEngine/vendor/assimp/bin/Release/assimp-vc141-mt.lib"
		}

		postbuildcommands 
		{
			'{COPY} "../UVKEngine/vendor/assimp/bin/Release/assimp-vc141-mt.dll" "%{cfg.targetdir}"'
		}