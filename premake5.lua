workspace "PixelBrahma"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "PixelBrahma/ThirdParty/GLFW/include"
IncludeDir["Glad"] = "PixelBrahma/ThirdParty/Glad/include"
IncludeDir["ImGui"] = "PixelBrahma/ThirdParty/ImGui"
IncludeDir["glm"] = "PixelBrahma/ThirdParty/glm"
IncludeDir["stb_image"] = "PixelBrahma/ThirdParty/stb_image"

group "Dependencies"
	include "PixelBrahma/ThirdParty/GLFW"
	include "PixelBrahma/ThirdParty/Glad"
	include "PixelBrahma/ThirdParty/ImGui"
	
group ""

project "PixelBrahma"
	location "PixelBrahma"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pbpch.h"
	pchsource "PixelBrahma/src/pbpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/ThirdParty/stb_image/**.h",
		"%{prj.name}/ThirdParty/stb_image/**.cpp",
		"%{prj.name}/ThirdParty/glm/glm/**.hpp",
		"%{prj.name}/ThirdParty/glm/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/ThirdParty/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"PB_PLATFORM_WINDOWS",
			"PB_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "PB_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "PB_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "PB_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"PixelBrahma/ThirdParty/spdlog/include",
		"PixelBrahma/src",
		"PixelBrahma/ThirdParty",
		"%{IncludeDir.glm}"
	}

	links
	{
		"PixelBrahma"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"PB_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "PB_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "PB_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "PB_DIST"
		runtime "Release"
		optimize "on"