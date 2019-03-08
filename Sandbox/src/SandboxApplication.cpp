#include <RetroGF.h>


// An example application using RGF



// A test layer.
class TestLayer : public RGF::Layer {

	public :


		TestLayer() : Layer("Test Layer") {}
		~TestLayer() {}

		void Init() override { RGF_TRACE(" %s Init Layer\n", m_DebugName.c_str()); }
		void ShutDown() override { RGF_TRACE(" %s Shutdown Layer\n", m_DebugName.c_str()); }

		void OnUpdate() override {}
		void OnEvent(RGF::Event& e) override {}


};


class SandboxApp : public RGF::Application {

	public :
		SandboxApp() {
			RGF_TRACE("Sandbox App was created!\n");

			PushLayer(new TestLayer());
		}
		~SandboxApp() {}

};

RGF::Application* RGF::CreateApplication() {
	return new SandboxApp;
}
