#pragma once

// ! The entrypoint only supports windows ! \\.
// ! This means that other platforms cannot run RGF yet ! \\.


// The entry point is handled via RGF.
// This means the entry point does not need to be defined in CLIENT side.
// This initializes a RGF application as well as extern libaries.



#ifdef RGF_PLATFORM_WINDOWS

	
	#include "RetroGF/Log.h"


	int main() {

		RGF::Log logger;
		logger.Init();


		// Testing the logger
		int a = 123;

		RGF_MSG("Hello World: %d\n", a);
		RGF_TRACE("Hello World: %d\n", a);
		RGF_WARN("Hello World: %d\n", a);
		RGF_ERROR("Hello World: %d\n", a);
		RGF_CRIT("Hello World: %d\n", a);

		std::cout << "-------" << std::endl;

		RGF_CORE_MSG("Hello World: %d\n", a);
		RGF_CORE_TRACE("Hello World: %d\n", a);
		RGF_CORE_WARN("Hello World: %d\n", a);
		RGF_CORE_ERROR("Hello World: %d\n", a);
		RGF_CORE_CRIT("Hello World: %d\n", a);


		auto app = RGF::CreateApplication();
		app->Run();
		delete app;

	}

#endif