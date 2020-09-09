#include <Pixelate.h>

#include "EditorLayer.h"

namespace Pixelate {


	class EditorApplication : public Application {

		public :
			EditorApplication() {
				PushLayer(instance = new EditorLayer);
			}
			~EditorApplication() {
				PopLayer(instance);
			}
		private :
			Layer* instance;

	};

	Application* CreateApplication() {
		return new EditorApplication;
	}

}
