#pragma once

// From SceneGraph.DX12
#include <Application.h>
// From SceneGraph
#include <Base/Scene.h>

namespace kitsune {
namespace scenegraph {
namespace dx12 {

	class ExampleApplication
		: public Bootstrap
	{
	public:
		/**
		 * This function is called before the main application is started.
		 * Here you can initialize settings, set window parameters
		 */
		void onInitializing(Application * app);
		void setWindowClass(WNDCLASSEX &WndClass);
		void onInitialized();
		void onTerminating();
		void onUpdate();
		void onRender();
	};

}
}
}
