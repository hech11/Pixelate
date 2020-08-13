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
IncludeDir["Glfw"] = "RetroGameFramework/vendor/Glfw"
IncludeDir["Glad"] = "RetroGameFramework/vendor/Glad"
IncludeDir["Imgui"] = "RetroGameFramework/vendor/Imgui"
IncludeDir["Glm"] = "RetroGameFramework/vendor/Glm"
IncludeDir["OpenAL_Soft"] = "RetroGameFramework/vendor/OpenAL_Soft"
IncludeDir["MiniMp3"] = "RetroGameFramework/vendor/MiniMp3"
IncludeDir["Box2D"] = "RetroGameFramework/vendor/Box2D"
IncludeDir["entt"] = "RetroGameFramework/vendor/entt"


IncludeDir["NativeFileDialog"] = "Retro-Editor/vendor/NativeFileDialog"



group "Dependencies/Core/Rendering"
	include "RetroGameFramework/vendor/Glfw"
	include "RetroGameFramework/vendor/Glad"
group "Dependencies/Core/Audio"
	include "RetroGameFramework/vendor/OpenAL_Soft"
group"Dependencies/Core/Physics"
	include "RetroGameFramework/vendor/Box2D"
group"Dependencies/Editor/"
	include "Retro-Editor/vendor/NativeFileDialog"
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
		"%{prj.name}/vendor/IMGUI/*.h",
		"%{prj.name}/vendor/IMGUI/*.cpp",
		"%{prj.name}/vendor/IMGUI/examples/imgui_impl_glfw.cpp",
		"%{prj.name}/vendor/IMGUI/examples/imgui_impl_glfw.h",
		"%{prj.name}/vendor/IMGUI/examples/imgui_impl_opengl3.cpp",
		"%{prj.name}/vendor/IMGUI/examples/imgui_impl_opengl3.h",
		"%{prj.name}/vendor/OPENAL_SOFT/include/AL/**h",
		"%{prj.name}/vendor/OPENAL_SOFT/src/common/**.h",
		"%{prj.name}/vendor/MINIMP3/**.h",
		"%{prj.name}/vendor/LIBOGG/include/**.h",
		"%{prj.name}/vendor/VORBIS/include/**.h",
		"%{prj.name}/vendor/VORBIS/lib/**.h",
		"%{prj.name}/vendor/entt/**.hpp"
	}


	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"NOMINMAX",
		"AL_LIBTYPE_STATIC"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor",
		"%{IncludeDir.Glfw}/include",
		"%{IncludeDir.Glad}/include",
		"%{IncludeDir.Glm}",
		"%{IncludeDir.Imgui}",
		"%{IncludeDir.OpenAL_Soft}/include",
		"%{IncludeDir.OpenAL_Soft}/src/common",
		"%{IncludeDir.OpenAL_Soft}/src/alc",
		"%{IncludeDir.OpenAL_Soft}/src/al",
		"%{IncludeDir.MiniMp3}/",
		"%{IncludeDir.Box2D}/include",
		"%{IncludeDir.entt}/vendor/entt/"


	}

	links 
	{ 
		"Glad",
		"Glfw",
		"opengl32.lib",
		"OpenAL_Soft",
		"Box2D",
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
		buildoptions { "/JMC", "/permissive-",  "/GS", "/W3", "/Zc:wchar_t",  "/ZI", "/Gm-", "/Od", "/sdl", "/Zc:inline", "/fp:precise", "/errorReport:prompt", "/WX-", "/Zc:forScope", "/RTC1", "/Gd", "/MTd", "/FC", "/EHsc", "/nologo", "/diagnostics:column"}



	filter "configurations:Release"
		defines { 
			"RGF_RELEASE",
			"RGF_USE_IMGUI",

		}
		runtime "Release"
		optimize "on"
		buildoptions {"/permissive-", "/MP", "/GS", "/GL", "/W0", "/Gy", "/Zc:wchar_t",  "/Zi", "/Gm-", "/O2", "/Ob2", "/sdl", "/Zc:inline", "/fp:fast", "/errorReport:prompt", "/WX-", "/Zc:forScope", "/Gd", "/Oi", "/MT", "/FC", "/EHsc", "/nologo", "/diagnostics:column"}

	filter "configurations:Distribute"
		defines
		{
			 "RGF_DISTRIBUTE",
			 "RGF_USE_CUSTOM_MEM_ALLOC"

		}
		runtime "Release"
		optimize "on"
		buildoptions {"/permissive-", "/MP", "/GS", "/GL", "/W0", "/Gy", "/Zc:wchar_t",  "/Zi", "/Gm-", "/O2", "/Ob2", "/sdl", "/Zc:inline", "/fp:fast", "/errorReport:prompt", "/WX-", "/Zc:forScope", "/Gd", "/Oi", "/MT", "/FC", "/EHsc", "/nologo", "/diagnostics:column"}



project "Retro-Editor"
	location "Retro-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/*.cpp",
		"%{prj.name}/src/*.h"
	}

	includedirs
	{
		"RetroGameFramework/src",
		"RetroGameFramework/vendor",
		"%{IncludeDir.Glm}",
		"%{IncludeDir.OpenAL_Soft}/include",
		"%{IncludeDir.OpenAL_Soft}/src/common",
		"%{IncludeDir.OpenAL_Soft}/src/alc",
		"%{IncludeDir.OpenAL_Soft}/src/al",
		"%{IncludeDir.MiniMp3}/",
		"%{IncludeDir.Box2D}/include",
		"%{IncludeDir.NativeFileDialog}src/include",
		"%{IncludeDir.entt}/vendor/entt/"

	}

	links
	{
		"NativeFileDialog",
		"RetroGameFramework"
	}
	
	filter "system:windows"
		systemversion "latest"


		defines
		{
			"_CRT_SECURE_NO_WARNINGS",
			"NOMINMAX",
			"GLFW_INCLUDE_NONE",
			"RGF_PLATFORM_WINDOWS",
			"AL_LIBTYPE_STATIC"
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
		buildoptions { "/JMC", "/permissive-",  "/GS", "/W3", "/Zc:wchar_t",  "/ZI", "/Gm-", "/Od", "/sdl", "/Zc:inline", "/fp:precise", "/errorReport:prompt", "/WX-", "/Zc:forScope", "/RTC1", "/Gd", "/MTd", "/FC", "/EHsc", "/nologo", "/diagnostics:column"}

	filter "configurations:Release"
		defines 
		{
			"RGF_RELEASE",
			"RGF_USE_IMGUI",

		}
		runtime "Release"
		optimize "on"
		buildoptions {"/permissive-", "/MP", "/GS", "/GL", "/W0", "/Gy", "/Zc:wchar_t",  "/Zi", "/Gm-", "/O2", "/Ob2", "/sdl", "/Zc:inline", "/fp:fast", "/errorReport:prompt", "/WX-", "/Zc:forScope", "/Gd", "/Oi", "/MT", "/FC", "/EHsc", "/nologo", "/diagnostics:column"}

	filter "configurations:Distribute"
		defines
		{
			 "RGF_DISTRIBUTE"

		}
		runtime "Release"
		optimize "on"
		buildoptions {"/permissive-", "/MP", "/GS", "/GL", "/W0", "/Gy", "/Zc:wchar_t",  "/Zi", "/Gm-", "/O2", "/Ob2", "/sdl", "/Zc:inline", "/fp:fast", "/errorReport:prompt", "/WX-", "/Zc:forScope", "/Gd", "/Oi", "/MT", "/FC", "/EHsc", "/nologo", "/diagnostics:column"}







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
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/*.cpp",
		"%{prj.name}/src/*.h"
	}

	includedirs
	{
		"RetroGameFramework/src",
		"RetroGameFramework/vendor",
		"%{IncludeDir.Glm}",
		"%{IncludeDir.OpenAL_Soft}/include",
		"%{IncludeDir.OpenAL_Soft}/src/common",
		"%{IncludeDir.OpenAL_Soft}/src/alc",
		"%{IncludeDir.OpenAL_Soft}/src/al",
		"%{IncludeDir.MiniMp3}/",
		"%{IncludeDir.Box2D}/include",
		"%{IncludeDir.entt}/vendor/entt/"
		
	}

	links
	{
		"RetroGameFramework"
	}
	
	filter "system:windows"
		systemversion "latest"


		defines
		{
			"_CRT_SECURE_NO_WARNINGS",
			"NOMINMAX",
			"GLFW_INCLUDE_NONE",
			"RGF_PLATFORM_WINDOWS",
			"AL_LIBTYPE_STATIC"
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
		buildoptions { "/JMC", "/permissive-",  "/GS", "/W3", "/Zc:wchar_t",  "/ZI", "/Gm-", "/Od", "/sdl", "/Zc:inline", "/fp:precise", "/errorReport:prompt", "/WX-", "/Zc:forScope", "/RTC1", "/Gd", "/MTd", "/FC", "/EHsc", "/nologo", "/diagnostics:column"}

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
		buildoptions {"/permissive-", "/MP", "/GS", "/GL", "/W0", "/Gy", "/Zc:wchar_t",  "/Zi", "/Gm-", "/O2", "/Ob2", "/sdl", "/Zc:inline", "/fp:fast", "/errorReport:prompt", "/WX-", "/Zc:forScope", "/Gd", "/Oi", "/MT", "/FC", "/EHsc", "/nologo", "/diagnostics:column"}

	filter "configurations:Distribute"
		defines
		{
			 "RGF_DISTRIBUTE"

		}
		runtime "Release"
		optimize "on"
		buildoptions {"/permissive-", "/MP", "/GS", "/GL", "/W0", "/Gy", "/Zc:wchar_t",  "/Zi", "/Gm-", "/O2", "/Ob2", "/sdl", "/Zc:inline", "/fp:fast", "/errorReport:prompt", "/WX-", "/Zc:forScope", "/Gd", "/Oi", "/MT", "/FC", "/EHsc", "/nologo", "/diagnostics:column"}