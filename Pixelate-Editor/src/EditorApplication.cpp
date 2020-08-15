#include <Pixelate.h>

#include "EditorLayer.h"

namespace Pixelate {


	class EditorApplication : public Application {

		public :
			EditorApplication() {
				PushLayer(new EditorLayer);
			}
			~EditorApplication() {}


	};

	Application* CreateApplication() {
		return new EditorApplication;
	}

}
