#include <RetroGF.h>


class SandboxApp : public RGF::Application {

	public:	
		SandboxApp() {}
		~SandboxApp() {}

};



int main() {
	SandboxApp* app = new SandboxApp;
	app->Run();
	delete app;
}