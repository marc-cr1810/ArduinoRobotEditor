include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "ArduinoRobotEditor"
	architecture "x86_64"
	startproject "Editor"

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
	include "Editor/vendor/GLFW"
	include "Editor/vendor/Glad"
	include "Editor/vendor/imgui"
group ""

group "Core"
	include "Editor"
group ""