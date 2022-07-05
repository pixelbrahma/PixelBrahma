workspace "PixelBrahma"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "PixelBrahma"
	location "PixelBrahma"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pbpch.h"
	pchsource "PixelBrahma/src/pbpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/ThirdParty/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"PB_PLATFORM_WINDOWS",
			"PB_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "PB_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "PB_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "PB_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

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
		"PixelBrahma/src"
	}

	links
	{
		"PixelBrahma"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"PB_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "PB_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "PB_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "PB_DIST"
		optimize "On"