#pragma once
// ! Only supports windows ! \\.


// The entry point is handled via RGF.
// This means the entry point does not need to be defined in CLIENT side.
// This initializes a RGF application as well as extern libaries.



#ifdef RGF_PLATFORM_WINDOWS

	// This is extern is declared in the "Application.h" file.
	extern RGF::Application* RGF::CreateApplication(); // Should return a Heap allocated application.

	int main() {
		auto app = RGF::CreateApplication();
		app->Run();
		delete app;

	}

#endif