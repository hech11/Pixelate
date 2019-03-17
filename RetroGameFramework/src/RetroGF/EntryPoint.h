#pragma once

// ! The entrypoint only supports windows ! \\.
// ! This means that other platforms cannot run RGF yet ! \\.


// The entry point is handled via RGF.
// This means the entry point does not need to be defined in CLIENT side.
// This initializes a RGF application as well as extern libaries.



#ifdef RGF_PLATFORM_WINDOWS

	
	#include "RetroGF/Log.h"


	int main() {
		using namespace RGF;
#ifndef RGF_DISTRIBUTE
		Log Logger;
		Logger.Init();
#endif

		FileSystem FileSys;

		auto App = RGF::CreateApplication();
		App->Run();
		delete App;

	}

#endif