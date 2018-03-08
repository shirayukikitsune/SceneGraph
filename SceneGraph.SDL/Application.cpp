#include "Application.h"

#include "Events/SDLEventHandler.h"

#include <cassert>
#include <SDL2/SDL.h>

using kitsune::scenegraph::sdl::Application;

Application::Application(kitsune::scenegraph::sdl::Bootstrap *Bootstrap)
	: Bootstrap(Bootstrap), WindowTitle("Kitsune Scenegraph SDL Application")
{
    Graphics = nullptr;
}

Application::~Application()
{
    SDL_Quit();
}

int Application::run()
{
    Uint32 lastTick = SDL_GetTicks();

    while (Bootstrap->shouldRun()) {
        Uint32 currentTick = SDL_GetTicks();
        std::chrono::milliseconds frameTime(currentTick - lastTick);
        lastTick = currentTick;

        EventHandler.processEvents();
        
        Bootstrap->onUpdate(frameTime);
        
        Graphics->clear();

        Bootstrap->onRender();

        Graphics->present();
    }

    return Bootstrap->returnCode();
}

void Application::setWindowTitle(const char *Title)
{
    if (this->Graphics) {
        this->Graphics->setWindowTitle(Title);
    }

    this->WindowTitle = Title;
}

void Application::createGraphics()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    int width, height;
    this->Bootstrap->getWindowDimensions(width, height);

    this->Graphics = new kitsune::scenegraph::sdl::Graphics(width, height);
    this->Graphics->setWindowTitle(this->WindowTitle.c_str());
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
