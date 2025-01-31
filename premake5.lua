include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

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

group "Dependencies"
	include "vendor/premake"
	include "PixelBrahma/ThirdParty/GLFW"
	include "PixelBrahma/ThirdParty/Glad"
	include "PixelBrahma/ThirdParty/ImGui"
	include "PixelBrahma/ThirdParty/yaml-cpp"
	include "PixelBrahma/ThirdParty/Box2D"
group ""

group "Core"
	include "PixelBrahma"
	include "PBScriptCore"
group ""

group "Tools"
	include "PBEditor"
group ""

group "Misc"
	include "Sandbox"
group ""