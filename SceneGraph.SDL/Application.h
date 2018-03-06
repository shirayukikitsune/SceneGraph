#pragma once

#include "Bootstrap.h"
#include "Graphics.h"
#include "Events/SDLEventHandler.h"

#include <string>

namespace kitsune {
namespace scenegraph {
namespace sdl {

	class Application
	{
	public:
		Application(Bootstrap *Bootstrap);
        ~Application();
        
        const char * getWindowTitle() { return WindowTitle.c_str(); }
		void setWindowTitle(const char * Title);

        int run();
        
        void createGraphics();
        Graphics* getGraphics() { return Graphics; }

        kitsune::scenegraph::events::EventHandler * getEventHandler() { return &EventHandler; }

	protected:
        Bootstrap *Bootstrap;
        Graphics *Graphics;

        events::SDLEventHandler EventHandler;

        std::string WindowTitle;
	};

}
}
}
