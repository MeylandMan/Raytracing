include "./Thirdparty/premake/solution_items.lua"

workspace "RayTracing"
	architecture "x86_64"
	startproject "Application"
	configurations { "Debug", "Release" }
	solution_items { ".editorconfig" }
	flags { "MultiProcessorCompile" }


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group("")

group("Core")
	include "Application"
	
group("")

group("Dependencies")
	include "Thirdparty/Glad"
	include "Thirdparty/premake"
	include "Thirdparty/GLFW"
	include "Thirdparty/imgui"