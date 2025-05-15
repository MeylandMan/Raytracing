project "Application"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"src/**.h",
		"src/**.cpp",
		"%{wks.location}/Thirdparty/glm/glm/**.hpp",
		"%{wks.location}/Thirdparty/glm/glm/**.inl",
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs {
		"src",
		"%{wks.location}/Thirdparty/GLFW/include",
		"%{wks.location}/Thirdparty/imgui",
		"%{wks.location}/Thirdparty/glm"
	}

	links
	{
		"GLFW",
		"ImGui",
	}
	
	flags { "NoPCH" }
	
	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "RAYTRACING_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "RAYTRACING_RELEASE"
		runtime "Release"
		optimize "On"