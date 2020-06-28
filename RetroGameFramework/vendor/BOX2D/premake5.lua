project "BOX2D"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"


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
		"src/collision/**.cpp",
		"src/common/**.cpp",
		"src/dynamics/**.cpp",
		"src/rope/**.cpp",
		"include/box2d/**.h",
	}
	
	includedirs
	{

		"include",
		"src",
	}
	filter "system:windows"
		systemversion "latest"
		staticruntime "On"



	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Distribute"
		runtime "Release"
		optimize "on"
