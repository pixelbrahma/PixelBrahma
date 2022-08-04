project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/PixelBrahma/ThirdParty/spdlog/include",
		"%{wks.location}/PixelBrahma/src",
		"%{wks.location}/PixelBrahma/ThirdParty",
		"%{IncludeDir.glm}",
		"%{IncludeDir.enTT}"
	}

	links
	{
		"PixelBrahma"
	}

	filter "system:windows"
		systemversion "latest"

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