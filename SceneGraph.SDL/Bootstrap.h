#pragma once

#include <chrono>
#include <memory>

namespace kitsune::scenegraph::sdl {

    class Application;

    class Bootstrap {
    public:
        virtual ~Bootstrap() = default;

        virtual void onInitializing(Application *Application) = 0;

        virtual void onInitialized() = 0;

        virtual void onTerminating() = 0;

        virtual void onUpdate(const std::chrono::milliseconds &frameTime) = 0;

        virtual void onRender() = 0;

        virtual bool shouldRun() = 0;

        virtual int returnCode() = 0;

        virtual void getWindowDimensions(int &width, int &height) = 0;

    protected:
    };

    extern std::unique_ptr<Bootstrap> GetApplicationInstance();

}
