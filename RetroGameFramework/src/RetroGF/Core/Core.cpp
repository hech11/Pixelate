#include "RGFpch.h"
#include "Core.h"


#include "Log.h"

#define RGF_VERSION_ID "v0.1alpha"

namespace RGF {



	void InitEngine() {

#ifndef RGF_DISTRIBUTE
		Log::Init();
#endif

		RGF_CORE_TRACE("--- RetroGameFramework ---\n");
		RGF_CORE_TRACE("Version: %s\n\n\n", RGF_VERSION_ID);
	}
	void ShutdownEngine() {
		RGF_CORE_TRACE("Shutting down..\n");
	}

}