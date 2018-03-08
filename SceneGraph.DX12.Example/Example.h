#pragma once

// From SceneGraph.DX12
#include <Bootstrap.h>
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
		void onInitializing();
		void onInitialized();
		void onTerminating();
		void onUpdate();
		void onRender();
	};

}
}
}
