#pragma once

// ! The entrypoint only supports windows ! \\.
// ! This means that other platforms cannot run RGF yet ! \\.


// The entry point is handled via RGF.
// This means the entry point does not need to be defined in CLIENT side.
// This initializes a RGF application as well as extern libraries.


#ifdef RGF_PLATFORM_WINDOWS

	
	#include "RetroGF/Debug/Instrumentor.h"
	#include "RetroGF/Core/Log.h"
	#include "RetroGF/Core/Core.h"


	int main() {

		#ifdef RGF_DEBUG
		RGF_PROFILE_BEGIN_SESSION("StartUp-Debug", "logs/Profling/Debug/RGFProfile-StartUp-Debug.json");
		#endif
		#ifdef RGF_RELEASE
		RGF_PROFILE_BEGIN_SESSION("StartUp-Release", "logs/Profling/Release/RGFProfile-StartUp-Release.json");
		#endif
		#ifdef RGF_DISTRIBUTE
		RGF_PROFILE_BEGIN_SESSION("StartUp-Distribute", "logs/Profling/Distribute/RGFProfile-StartUp-Distribute.json");
		#endif


		RGF::InitEngine();
		using namespace RGF;

		RGF_CORE_MSG("Creating application...\n");
		auto App = RGF::CreateApplication();

		RGF_PROFILE_END_SESSION();

		#ifdef RGF_DEBUG
		RGF_PROFILE_BEGIN_SESSION("Runtime-Debug", "logs/Profling/Debug/RGFProfile-Runtime-Debug.json");
		#endif
		#ifdef RGF_RELEASE
		RGF_PROFILE_BEGIN_SESSION("Runtime-Release", "logs/Profling/Release/RGFProfile-Runtime-Release.json");
		#endif
		#ifdef RGF_DISTRIBUTE
		RGF_PROFILE_BEGIN_SESSION("Runtime-Distribute", "logs/Profling/Distribute/RGFProfile-Runtime-Distribute.json");
		#endif

		App->Run();
		RGF_PROFILE_END_SESSION();


		#ifdef RGF_DEBUG
		RGF_PROFILE_BEGIN_SESSION("Shutdown-Debug", "logs/Profling/Debug/RGFProfile-Shutdown-Debug.json");
		#endif
		#ifdef RGF_RELEASE
		RGF_PROFILE_BEGIN_SESSION("Shutdown-Release", "logs/Profling/Release/RGFProfile-Shutdown-Release.json");
		#endif
		#ifdef RGF_DISTRIBUTE
		RGF_PROFILE_BEGIN_SESSION("Shutdown-Distribute", "logs/Profling/Distribute/RGFProfile-Shutdown-Distribute.json");
		#endif
		RGF::ShutdownEngine();

		delete App;
		RGF_PROFILE_END_SESSION();

	}

#endif