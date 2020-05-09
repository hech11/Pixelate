project "OPENAL_SOFT"
	kind "StaticLib"
	language "C++"
	cppdialect "C++14"
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
		"src/al",
		"src/alc",
		"src/common",
		"include/"
	}

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	excludes
	{
		"src/alc/mixer/mixer_neon.cpp",
	}

	defines
	{
		"AL_LIBTYPE_STATIC"
	}
	
	filter "system:windows"
		systemversion "latest"

		excludes
		{
			"src/alc/backends/oss.cpp",
			"src/alc/backends/oss.h",
			"src/alc/backends/opensl.cpp",
			"src/alc/backends/opensl.h",
			"src/alc/backends/jack.cpp",
			"src/alc/backends/jack.h",
			"src/alc/backends/portaudio.cpp",
			"src/alc/backends/portaudio.h",
			"src/alc/backends/alsa.cpp",
			"src/alc/backends/alsa.h",
			"src/alc/backends/coreaudio.cpp",
			"src/alc/backends/coreaudio.h",
			"src/alc/backends/sdl2.cpp",
			"src/alc/backends/sdl2.h",
			"src/alc/backends/sndio.cpp",
			"src/alc/backends/sndio.h",
			"src/alc/backends/solaris.cpp",
			"src/alc/backends/solaris.h",
			"src/alc/backends/pulseaudio.h",
			"src/alc/backends/pulseaudio.cpp",
			"src/alc/backends/oboe.h",
			"src/alc/backends/oboe.cpp"

		}

		defines
		{
			"WIN32",
			"_WINDOWS",
			"AL_BUILD_LIBRARY",
			"AL_ALEXT_PROTOTYPES",
			"_WIN32",
			"_WIN32_WINNT=0x0502",
			"_CRT_SECURE_NO_WARNINGS",
			"NOMINMAX",
			"CMAKE_INTDIR=\"Debug\"",
			"OpenAL_EXPORTS"
		}

		links
		{
			"winmm"
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