project "yaml-cpp"
	kind "StaticLib"
	language "C"
	staticruntime "off"


	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")


	configurations
	{
		"Debug",
		"Release",
		"Distribute"
	}

	files
	{
		"src/**.cpp",
		"src/**.h",
		"include/yaml-cpp/**.h"
	}
	includedirs
	{
		"include/"
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "off"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Distribute"
		runtime "Release"
		optimize "on"
