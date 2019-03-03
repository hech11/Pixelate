#include <RetroGF.h>

// An example application using RGF


class SandboxApp : public RGF::Application {

	public:	
		SandboxApp() {
			RGF_TRACE("Sandbox App was created!");
		}
		~SandboxApp() {}

};

RGF::Application* RGF::CreateApplication() {
	return new SandboxApp;
}
