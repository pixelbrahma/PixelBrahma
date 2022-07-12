workspace "PixelBrahma"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "PixelBrahma/ThirdParty/GLFW/include"
IncludeDir["Glad"] = "PixelBrahma/ThirdParty/Glad/include"
IncludeDir["ImGui"] = "PixelBrahma/ThirdParty/ImGui"
IncludeDir["glm"] = "PixelBrahma/ThirdParty/glm"

group "Dependencies"
	include "PixelBrahma/ThirdParty/GLFW"
	include "PixelBrahma/ThirdParty/Glad"
	include "PixelBrahma/ThirdParty/ImGui"
group ""

project "PixelBrahma"
	location "PixelBrahma"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pbpch.h"
	pchsource "PixelBrahma/src/pbpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/ThirdParty/glm/glm/**.hpp",
		"%{prj.name}/ThirdParty/glm/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/ThirdParty/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"PB_PLATFORM_WINDOWS",
			"PB_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines "PB_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "PB_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "PB_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

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
		"%{IncludeDir.glm}"
	}

	links
	{
		"PixelBrahma"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"PB_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "PB_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "PB_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "PB_DIST"
		runtime "Release"
		optimize "On"