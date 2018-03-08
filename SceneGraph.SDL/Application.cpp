#include "Application.h"

#include "Events/SDLEventHandler.h"

#include <cassert>
#include <SDL2/SDL.h>

using kitsune::scenegraph::sdl::Application;

Application::Application(kitsune::scenegraph::sdl::Bootstrap *Bootstrap)
	: _Bootstrap(Bootstrap), WindowTitle("Kitsune Scenegraph SDL Application")
{
    _Graphics = nullptr;
}

Application::~Application()
{
    SDL_Quit();
}

int Application::run()
{
    Uint32 lastTick = SDL_GetTicks();

    while (_Bootstrap->shouldRun()) {
        Uint32 currentTick = SDL_GetTicks();
        std::chrono::milliseconds frameTime(currentTick - lastTick);
        lastTick = currentTick;

        EventHandler.processEvents();
        
        _Bootstrap->onUpdate(frameTime);
        
        _Graphics->clear();

        _Bootstrap->onRender();

        _Graphics->present();
    }

    return _Bootstrap->returnCode();
}

void Application::setWindowTitle(const char *Title)
{
    if (_Graphics) {
        _Graphics->setWindowTitle(Title);
    }

    WindowTitle = Title;
}

void Application::createGraphics()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    int width, height;
    _Bootstrap->getWindowDimensions(width, height);

    _Graphics = new kitsune::scenegraph::sdl::Graphics(width, height);
    _Graphics->setWindowTitle(this->WindowTitle.c_str());
}

int main(int argc, char ** argv)
{
    using namespace kitsune::scenegraph::sdl;

    assert(AppBootstrap != nullptr && "AppBootstrap is not set");

    Application * App = new Application(AppBootstrap);

    AppBootstrap->onInitializing(App);

    App->createGraphics();

    AppBootstrap->onInitialized();

    int retCode = App->run();

    AppBootstrap->onTerminating();

    delete AppBootstrap;
    delete App;

    return retCode;
}
