#pragma once

// ! The entrypoint only supports windows ! \\.
// ! This means that other platforms cannot run PX yet ! \\.


// The entry point is handled via PX.
// This means the entry point does not need to be defined in CLIENT side.
// This initializes a PX application as well as extern libraries.


#ifdef PX_PLATFORM_WINDOWS

	
	#include "Pixelate/Debug/Instrumentor.h"
	#include "Pixelate/Core/Log.h"
	#include "Pixelate/Core/Core.h"


	int main() {

		#ifdef PX_DEBUG
		PX_PROFILE_BEGIN_SESSION("StartUp-Debug", "logs/Profling/Debug/PXProfile-StartUp-Debug.json");
		#endif
		#ifdef PX_RELEASE
		PX_PROFILE_BEGIN_SESSION("StartUp-Release", "logs/Profling/Release/PXProfile-StartUp-Release.json");
		#endif
		#ifdef PX_DISTRIBUTE
		PX_PROFILE_BEGIN_SESSION("StartUp-Distribute", "logs/Profling/Distribute/PXProfile-StartUp-Distribute.json");
		#endif


		Pixelate::InitEngine();
		using namespace Pixelate;

		PX_CORE_MSG("Creating application...\n");
		auto App = Pixelate::CreateApplication();

		PX_PROFILE_END_SESSION();

		#ifdef PX_DEBUG
		PX_PROFILE_BEGIN_SESSION("Runtime-Debug", "logs/Profling/Debug/PXProfile-Runtime-Debug.json");
		#endif
		#ifdef PX_RELEASE
		PX_PROFILE_BEGIN_SESSION("Runtime-Release", "logs/Profling/Release/PXProfile-Runtime-Release.json");
		#endif
		#ifdef PX_DISTRIBUTE
		PX_PROFILE_BEGIN_SESSION("Runtime-Distribute", "logs/Profling/Distribute/PXProfile-Runtime-Distribute.json");
		#endif

		App->Run();
		PX_PROFILE_END_SESSION();


		#ifdef PX_DEBUG
		PX_PROFILE_BEGIN_SESSION("Shutdown-Debug", "logs/Profling/Debug/PXProfile-Shutdown-Debug.json");
		#endif
		#ifdef PX_RELEASE
		PX_PROFILE_BEGIN_SESSION("Shutdown-Release", "logs/Profling/Release/PXProfile-Shutdown-Release.json");
		#endif
		#ifdef PX_DISTRIBUTE
		PX_PROFILE_BEGIN_SESSION("Shutdown-Distribute", "logs/Profling/Distribute/PXProfile-Shutdown-Distribute.json");
		#endif
		Pixelate::ShutdownEngine();

		delete App;
		PX_PROFILE_END_SESSION();

	}

#endif