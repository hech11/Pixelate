workspace "Pixelate"
	architecture "x86_64"
	startproject "Pixelate-Editor"

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
IncludeDir["Glfw"] = "Pixelate/vendor/Glfw"
IncludeDir["Glad"] = "Pixelate/vendor/Glad"
IncludeDir["Imgui"] = "Pixelate/vendor/Imgui"
IncludeDir["Glm"] = "Pixelate/vendor/Glm"
IncludeDir["OpenAL_Soft"] = "Pixelate/vendor/OpenAL_Soft"
IncludeDir["MiniMp3"] = "Pixelate/vendor/MiniMp3"
IncludeDir["Box2D"] = "Pixelate/vendor/Box2D"
IncludeDir["entt"] = "Pixelate/vendor/entt"


IncludeDir["NativeFileDialog"] = "Pixelate-Editor/vendor/NativeFileDialog"



group "Dependencies/Core/Rendering"
	include "Pixelate/vendor/Glfw"
	include "Pixelate/vendor/Glad"
group "Dependencies/Core/Audio"
	include "Pixelate/vendor/OpenAL_Soft"
group"Dependencies/Core/Physics"
	include "Pixelate/vendor/Box2D"
group"Dependencies/Editor/"
	include "Pixelate-Editor/vendor/NativeFileDialog"
group ""

project "Pixelate"
	location "Pixelate"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "PXpch.h"
	pchsource "Pixelate/src/PXpch.cpp"

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
			"PX_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines { 
			"PX_DEBUG",
			"PX_USE_IMGUI",
			"PX_USE_LOGGING",
			"PX_USE_INSTRUMENTOR",

		}
		runtime "Debug"
		symbols "on"
		buildoptions { "/JMC", "/permissive-",  "/GS", "/W3", "/Zc:wchar_t",  "/ZI", "/Gm-", "/Od", "/sdl", "/Zc:inline", "/fp:precise", "/errorReport:prompt", "/WX-", "/Zc:forScope", "/RTC1", "/Gd", "/MTd", "/FC", "/EHsc", "/nologo", "/diagnostics:column"}



	filter "configurations:Release"
		defines { 
			"PX_RELEASE",
			"PX_USE_IMGUI",

		}
		runtime "Release"
		optimize "on"
		buildoptions {"/permissive-", "/MP", "/GS", "/GL", "/W0", "/Gy", "/Zc:wchar_t",  "/Zi", "/Gm-", "/O2", "/Ob2", "/sdl", "/Zc:inline", "/fp:fast", "/errorReport:prompt", "/WX-", "/Zc:forScope", "/Gd", "/Oi", "/MT", "/FC", "/EHsc", "/nologo", "/diagnostics:column"}

	filter "configurations:Distribute"
		defines
		{
			 "PX_DISTRIBUTE",
			 "PX_USE_CUSTOM_MEM_ALLOC"

		}
		runtime "Release"
		optimize "on"
		buildoptions {"/permissive-", "/MP", "/GS", "/GL", "/W0", "/Gy", "/Zc:wchar_t",  "/Zi", "/Gm-", "/O2", "/Ob2", "/sdl", "/Zc:inline", "/fp:fast", "/errorReport:prompt", "/WX-", "/Zc:forScope", "/Gd", "/Oi", "/MT", "/FC", "/EHsc", "/nologo", "/diagnostics:column"}



project "Pixelate-Editor"
	location "Pixelate-Editor"
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
		"Pixelate/src",
		"Pixelate/vendor",
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
		"Pixelate-Scripting/Pixelate-Scripting/mono-2.0-sgen.lib",
		"Pixelate"
	}
	
	filter "system:windows"
		systemversion "latest"


		defines
		{
			"_CRT_SECURE_NO_WARNINGS",
			"NOMINMAX",
			"GLFW_INCLUDE_NONE",
			"PX_PLATFORM_WINDOWS",
			"AL_LIBTYPE_STATIC"
		}
		
	filter "configurations:Debug"
		defines { 
			"PX_DEBUG",
			"PX_USE_IMGUI",
			"PX_USE_LOGGING",
			"PX_USE_INSTRUMENTOR",

		}
		runtime "Debug"
		symbols "on"
		buildoptions { "/JMC", "/permissive-",  "/GS", "/W3", "/Zc:wchar_t",  "/ZI", "/Gm-", "/Od", "/sdl", "/Zc:inline", "/fp:precise", "/errorReport:prompt", "/WX-", "/Zc:forScope", "/RTC1", "/Gd", "/MTd", "/FC", "/EHsc", "/nologo", "/diagnostics:column"}

	filter "configurations:Release"
		defines 
		{
			"PX_RELEASE",
			"PX_USE_IMGUI",

		}
		runtime "Release"
		optimize "on"
		buildoptions {"/permissive-", "/MP", "/GS", "/GL", "/W0", "/Gy", "/Zc:wchar_t",  "/Zi", "/Gm-", "/O2", "/Ob2", "/sdl", "/Zc:inline", "/fp:fast", "/errorReport:prompt", "/WX-", "/Zc:forScope", "/Gd", "/Oi", "/MT", "/FC", "/EHsc", "/nologo", "/diagnostics:column"}

	filter "configurations:Distribute"
		defines
		{
			 "PX_DISTRIBUTE"

		}
		runtime "Release"
		optimize "on"
		buildoptions {"/permissive-", "/MP", "/GS", "/GL", "/W0", "/Gy", "/Zc:wchar_t",  "/Zi", "/Gm-", "/O2", "/Ob2", "/sdl", "/Zc:inline", "/fp:fast", "/errorReport:prompt", "/WX-", "/Zc:forScope", "/Gd", "/Oi", "/MT", "/FC", "/EHsc", "/nologo", "/diagnostics:column"}






project "Pixelate-Scripting"
	location "Pixelate-Scripting"
	kind "SharedLib"
	language "C#"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	

	files
	{
		"%{prj.name}/src/**.cs", 
	}



project "Sandbox-CSharpApp"
	location "Sandbox-CSharpApp"
	kind "ConsoleApp"
	language "C#"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	

	files
	{
		"%{prj.name}/src/**.cs", 
	}


	links
	{
		"Pixelate-Scripting"
	}



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
		"Pixelate/src",
		"Pixelate/vendor",
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
		"Pixelate"
	}
	
	filter "system:windows"
		systemversion "latest"


		defines
		{
			"_CRT_SECURE_NO_WARNINGS",
			"NOMINMAX",
			"GLFW_INCLUDE_NONE",
			"PX_PLATFORM_WINDOWS",
			"AL_LIBTYPE_STATIC"
		}
		
	filter "configurations:Debug"
		defines { 
			"PX_DEBUG",
			"PX_USE_IMGUI",
			"PX_USE_LOGGING",
			"PX_USE_INSTRUMENTOR",

		}
		runtime "Debug"
		symbols "on"
		buildoptions { "/JMC", "/permissive-",  "/GS", "/W3", "/Zc:wchar_t",  "/ZI", "/Gm-", "/Od", "/sdl", "/Zc:inline", "/fp:precise", "/errorReport:prompt", "/WX-", "/Zc:forScope", "/RTC1", "/Gd", "/MTd", "/FC", "/EHsc", "/nologo", "/diagnostics:column"}

	filter "configurations:Release"
		defines 
		{
			"PX_RELEASE",
			"PX_USE_IMGUI",
			"PX_USE_LOGGING",
			"PX_USE_INSTRUMENTOR",
		}
		runtime "Release"
		optimize "on"
		buildoptions {"/permissive-", "/MP", "/GS", "/GL", "/W0", "/Gy", "/Zc:wchar_t",  "/Zi", "/Gm-", "/O2", "/Ob2", "/sdl", "/Zc:inline", "/fp:fast", "/errorReport:prompt", "/WX-", "/Zc:forScope", "/Gd", "/Oi", "/MT", "/FC", "/EHsc", "/nologo", "/diagnostics:column"}

	filter "configurations:Distribute"
		defines
		{
			 "PX_DISTRIBUTE"

		}
		runtime "Release"
		optimize "on"
		buildoptions {"/permissive-", "/MP", "/GS", "/GL", "/W0", "/Gy", "/Zc:wchar_t",  "/Zi", "/Gm-", "/O2", "/Ob2", "/sdl", "/Zc:inline", "/fp:fast", "/errorReport:prompt", "/WX-", "/Zc:forScope", "/Gd", "/Oi", "/MT", "/FC", "/EHsc", "/nologo", "/diagnostics:column"}