project "LIBOGG"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")



	configurations {
		"Debug",
		"Release",
		"Distribute"
	}


    files {
        "include/**.h",
        "src/**.c"
    }

    includedirs {
        "include"
    }

    links {
        "LIBOGG"
    }

    defines {
        "LIBOGG_EXPORTS"
    }
    
    filter "system:windows"
        systemversion "latest"


        excludes {
            "lib/barkmel.c",
            "lib/misc.c",
            "lib/psytune.c",
            "lib/tone.c"
        }

        defines {
            "WIN32",
            "NDEBUG",
            "_WINDOWS",
            "_USRDLL"
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