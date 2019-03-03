#pragma once



// Common marcos that are used when developing RGF.

//TODO: i should really add the PCH here.


#ifdef RGF_PLATFORM_WINDOWS

	#ifdef RGF_BUILD_DLL
		#define RGF_API __declspec(dllexport)
	#else
		#define RGF_API __declspec(dllimport)
	#endif
#else
	#error RetroGameFramework only supports windows!
#endif
