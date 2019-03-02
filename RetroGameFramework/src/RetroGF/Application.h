#pragma once

#include "RetroGF/Core.h"


// This would be inherited from another class from CLIENT side.
// The application class is the main 'hub' of all RGF compondents.

// This would initialize the window, renderer, physics, ect.



namespace RGF {

	class RGF_API Application {

		public :
			Application();
			~Application();


			void Run();



	};

	// Has to be defined on CLIENT side in order to create a RGF application.
	Application* CreateApplication(); // Should return a heap allocated application.
}