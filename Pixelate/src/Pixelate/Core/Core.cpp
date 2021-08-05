#include "PXpch.h"
#include "Core.h"


#include "Log.h"

#define PX_VERSION_ID "v0.1alpha"

namespace Pixelate {



	void InitEngine() {

#ifndef PX_DISTRIBUTE
		Log::Init();
#endif

		PX_CORE_TRACE("--- Pixelate ---\n");
		PX_CORE_TRACE("Version: %s\n\n\n", PX_VERSION_ID);
	}
	void ShutdownEngine() {
		PX_CORE_TRACE("Shutting down..\n");
	}

	bool IsBigEndian() {
		int a = 1;
		return !((char*)&a);
	}
}