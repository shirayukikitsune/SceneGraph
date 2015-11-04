#pragma once

#include "Bootstrap.h"

namespace kitsune {
namespace scenegraph {
namespace dx12 {

	class Application
	{
	public:
		Application(Bootstrap *Bootstrap);
		~Application();

		void setWindowDimentions(LONG Width, LONG Height);

		LPCTSTR getWindowTitle() { return WindowTitle; }
		void setWindowTitle(LPCTSTR Title);

		void createWindow(HINSTANCE hInstance, int ShowCommand);

		int run();

	protected:
		HWND MainWindow;
		LONG WindowWidth;
		LONG WindowHeight;
		LPTSTR WindowTitle;
		Bootstrap *Bootstrap;

		static LRESULT CALLBACK WindowProc(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam);
	};

}
}
}
