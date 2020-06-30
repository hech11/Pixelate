#include <RetroGF.h>

#include "EditorLayer.h"

namespace RGF {


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
