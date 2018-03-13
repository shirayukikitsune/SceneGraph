#pragma once

// From SceneGraph.SDL
#include <Application.h>
// From SceneGraph
#include <Base/Scene.h>

#include <memory>

namespace kitsune {
namespace scenegraph {
namespace sdl {

	class ExampleApplication
		: public Bootstrap
	{
	public:
        ExampleApplication();

		void onInitializing(Application * Application) override;
		void onInitialized() override;
	    void onTerminating() override;
		void onUpdate(const std::chrono::milliseconds & frameTime) override;
		void onRender() override;
        
        bool shouldRun() override;
        int returnCode() override;

        void getWindowDimensions(int &width, int &height) override;

    private:
        std::shared_ptr<Scene> CurrentScene;
		std::shared_ptr<Node> Camera;
        Application *_Application;

        bool Run;
		float AspectRatio;

        float currentTime;
	};

}
}
}
