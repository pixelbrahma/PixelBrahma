include "./vendor/premake/premake_customization/solution_items.lua"

workspace "PixelBrahma"
	architecture "x86_64"
	startproject "PBEditor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/PixelBrahma/ThirdParty/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/PixelBrahma/ThirdParty/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/PixelBrahma/ThirdParty/ImGui"
IncludeDir["glm"] = "%{wks.location}/PixelBrahma/ThirdParty/glm"
IncludeDir["stb_image"] = "%{wks.location}/PixelBrahma/ThirdParty/stb_image"
IncludeDir["enTT"] = "%{wks.location}/PixelBrahma/ThirdParty/enTT/Include"
IncludeDir["yaml_cpp"] = "%{wks.location}/PixelBrahma/ThirdParty/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/PixelBrahma/ThirdParty/ImGuizmo"

group "Dependencies"
	include "vendor/premake"
	include "PixelBrahma/ThirdParty/GLFW"
	include "PixelBrahma/ThirdParty/Glad"
	include "PixelBrahma/ThirdParty/ImGui"
	include "PixelBrahma/ThirdParty/yaml-cpp"
group ""

include "PixelBrahma"
include "Sandbox"
include "PBEditor"