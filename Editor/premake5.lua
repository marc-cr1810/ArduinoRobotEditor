project "Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "epch.h"
	pchsource "src/epch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
		"vendor/imgui_nodes/**.h",
		"vendor/imgui_nodes/**.inl",
		"vendor/imgui_nodes/**.cpp",
		"vendor/imgui_textedit/**.h",
		"vendor/imgui_textedit/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"src",
		"vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.imgui_nodes}",
		"%{IncludeDir.imgui_textedit}",
		"%{IncludeDir.stb_image}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

		defines
		{
		}

		links
		{
			"%{Library.WinSock}",
			"%{Library.WinMM}",
			"%{Library.WinVersion}",
			"%{Library.BCrypt}",
		}

	filter "configurations:Debug"
		defines "ED_DEBUG"
		runtime "Debug"
		symbols "on"

		links
		{
			
		}

	filter "configurations:Release"
		defines "ED_RELEASE"
		runtime "Release"
		optimize "on"

		links
		{
			
		}

	filter "configurations:Dist"
		defines "ED_DIST"
		runtime "Release"
		optimize "on"

		links
		{
			
		}
