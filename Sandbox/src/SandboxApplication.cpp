#include <RetroGF.h>

// Example application using RGF


class SandboxApp : public RGF::Application {

	public:	
		SandboxApp() {}
		~SandboxApp() {}

};

RGF::Application* RGF::CreateApplication() {
	return new SandboxApp;
}
