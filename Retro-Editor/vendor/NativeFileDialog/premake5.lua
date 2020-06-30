project "NativeFileDialog"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	configurations
	{
		"Debug",
		"Release",
		"Distribute"
	}

	includedirs
	{
		"src/include/"
	}

	files
	{
		"src/**.h",
		"src/**.cpp",
		"src/*.h",
		"src/*.cpp",
		"src/*.c",
		"src/**.c"
			
	}
	
	filter "system:windows"
		systemversion "latest"

		excludes
		{
			"src/nfd_gtk.c",
			"src/nfd_zenity.c"
		}

		defines
		{
			"WIN32",
			"_WINDOWS",
		}


	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

filter "configurations:Distribute"
		runtime "Release"
		optimize "on"