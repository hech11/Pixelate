workspace "RetroGameFramework"
	architecture "x86_64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Distribute"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "RetroGameFramework/vendor/GLFW"
IncludeDir["GLAD"] = "RetroGameFramework/vendor/GLAD"
IncludeDir["IMGUI"] = "RetroGameFramework/vendor/IMGUI"
IncludeDir["GLM"] = "RetroGameFramework/vendor/GLM"
IncludeDir["STB_IMAGE"] = "RetroGameFramework/vendor/STB_IMAGE"

group "Dependencies"
	include "RetroGameFramework/vendor/GLFW"
	include "RetroGameFramework/vendor/GLAD"

group ""

project "RetroGameFramework"
	location "RetroGameFramework"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "RGFpch.h"
	pchsource "RetroGameFramework/src/RGFpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/STB/IMAGE**.h",
		"%{prj.name}/vendor/STB/IMAGE/**.cpp",
		"%{prj.name}/vendor/GLM/glm/**.hpp",
		"%{prj.name}/vendor/GLM/glm/**.inl",
		"%{prj.name}/vendor/GLAD/**.h",
		"%{prj.name}/vendor/GLAD/**.cpp",
		"%{prj.name}/vendor/IMGUI/imconfig.h",
		"%{prj.name}/vendor/IMGUI/imgui.h",
		"%{prj.name}/vendor/IMGUI/imgui.cpp",
		"%{prj.name}/vendor/IMGUI/imgui_draw.cpp",
		"%{prj.name}/vendor/IMGUI/imgui_internal.h",
		"%{prj.name}/vendor/IMGUI/imgui_widgets.cpp",
		"%{prj.name}/vendor/IMGUI/imstb_rectpack.h",
		"%{prj.name}/vendor/IMGUI/imstb_textedit.h",
		"%{prj.name}/vendor/IMGUI/imstb_truetype.h",
		"%{prj.name}/vendor/IMGUI/imgui_demo.cpp",
		"%{prj.name}/vendor/IMGUI/examples/imgui_impl_glfw.cpp",
		"%{prj.name}/vendor/IMGUI/examples/imgui_impl_glfw.h",
		"%{prj.name}/vendor/IMGUI/examples/imgui_impl_opengl3.cpp",
		"%{prj.name}/vendor/IMGUI/examples/imgui_impl_opengl3.h"

	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor"
	}

	links 
	{ 
		"GLAD",
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"RGF_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"RGF_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines { 
			"RGF_DEBUG",
			"RGF_USE_IMGUI",
			"RGF_USE_LOGGING",
			"RGF_USE_INSTRUMENTOR",

		}
		runtime "Debug"
		symbols "on"
		buildoptions { "/JMC", "/permissive-",  "/GS", "/W3", "/Zc:wchar_t",  "/ZI", "/Gm-", "/Od", "/sdl", "/Zc:inline", "/fp:precise", "/errorReport:prompt", "/WX-", "/Zc:forScope", "/RTC1", "/Gd", "/MDd", "/FC", "/EHsc", "/nologo", "/diagnostics:column"}



	filter "configurations:Release"
		defines { 
			"RGF_RELEASE",
			"RGF_USE_IMGUI",
			"RGF_USE_LOGGING",
			"RGF_USE_INSTRUMENTOR",

		}
		runtime "Release"
		optimize "on"
		buildoptions {"/permissive-", "/MP", "/GS", "/GL", "/W0", "/Gy", "/Zc:wchar_t",  "/Zi", "/Gm-", "/O2", "/Ob2", "/sdl", "/Zc:inline", "/fp:fast", "/errorReport:prompt", "/WX-", "/Zc:forScope", "/Gd", "/Oi", "/MD", "/FC", "/EHsc", "/nologo", "/diagnostics:column"}

	filter "configurations:Distribute"
		defines
		{
			 "RGF_DISTRIBUTE",
			 "RGF_USE_CUSTOM_MEM_ALLOC"

		}
		runtime "Release"
		optimize "on"
		buildoptions {"/permissive-", "/MP", "/GS", "/GL", "/W0", "/Gy", "/Zc:wchar_t",  "/Zi", "/Gm-", "/O2", "/Ob2", "/sdl", "/Zc:inline", "/fp:fast", "/errorReport:prompt", "/WX-", "/Zc:forScope", "/Gd", "/Oi", "/MD", "/FC", "/EHsc", "/nologo", "/diagnostics:column"}


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
		"RetroGameFramework/src",
		"RetroGameFramework/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLAD",
		"GLFW",
		"RetroGameFramework"
	}



	filter "system:windows"
		systemversion "latest"


		defines
		{
			"GLFW_INCLUDE_NONE",
			"RGF_PLATFORM_WINDOWS"
		}
		
	filter "configurations:Debug"
		defines { 
			"RGF_DEBUG",
			"RGF_USE_IMGUI",
			"RGF_USE_LOGGING",
			"RGF_USE_INSTRUMENTOR",

		}
		runtime "Debug"
		symbols "on"
		buildoptions { "/JMC", "/permissive-",  "/GS", "/W3", "/Zc:wchar_t",  "/ZI", "/Gm-", "/Od", "/sdl", "/Zc:inline", "/fp:precise", "/errorReport:prompt", "/WX-", "/Zc:forScope", "/RTC1", "/Gd", "/MDd", "/FC", "/EHsc", "/nologo", "/diagnostics:column"}

	filter "configurations:Release"
		defines 
		{
			"RGF_RELEASE",
			"RGF_USE_IMGUI",
			"RGF_USE_LOGGING",
			"RGF_USE_INSTRUMENTOR",
		}
		runtime "Release"
		optimize "on"
		buildoptions {"/permissive-", "/MP", "/GS", "/GL", "/W0", "/Gy", "/Zc:wchar_t",  "/Zi", "/Gm-", "/O2", "/Ob2", "/sdl", "/Zc:inline", "/fp:fast", "/errorReport:prompt", "/WX-", "/Zc:forScope", "/Gd", "/Oi", "/MD", "/FC", "/EHsc", "/nologo", "/diagnostics:column"}

	filter "configurations:Distribute"
		defines
		{
			 "RGF_DISTRIBUTE"

		}
		runtime "Release"
		optimize "on"
		buildoptions {"/permissive-", "/MP", "/GS", "/GL", "/W0", "/Gy", "/Zc:wchar_t",  "/Zi", "/Gm-", "/O2", "/Ob2", "/sdl", "/Zc:inline", "/fp:fast", "/errorReport:prompt", "/WX-", "/Zc:forScope", "/Gd", "/Oi", "/MD", "/FC", "/EHsc", "/nologo", "/diagnostics:column"}