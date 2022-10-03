#include <Pixelate.h>


#include "SandboxLayer.h"

namespace Pixelate {

	
	class SandboxApplication : public Application {

		public :
			SandboxApplication() {
				PushLayer(instance = new SandboxLayer);
			}
			~SandboxApplication() {
				PopLayer(instance);
			}
		private :
			Layer* instance;

	};

	Application* CreateApplication() {
		return new SandboxApplication;
	}

}