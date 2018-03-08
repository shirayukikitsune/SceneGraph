#pragma once

#include "Bootstrap.h"

namespace kitsune {
namespace scenegraph {
namespace dx12 {

    class Device;

	class Application
	{
    private:
		Application(Bootstrap *Bootstrap);
		~Application();
        static Application* Instance;

    public:
        static Application* prepareInstance();
        static Application* getInstance();

		void setWindowDimentions(LONG Width, LONG Height);

		LPCTSTR getWindowTitle() { return WindowTitle; }
		void setWindowTitle(LPCTSTR Title);

		void createWindow(HINSTANCE hInstance, int ShowCommand);
        void createRenderer();

		int run();

        Device * getDevice() { return RenderDevice; }

	protected:
		HWND MainWindow;
		LONG WindowWidth;
		LONG WindowHeight;
		LPTSTR WindowTitle;
		Bootstrap *Bootstrap;
        Device *RenderDevice;

		static LRESULT CALLBACK WindowProc(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam);
	};

}
}
}
