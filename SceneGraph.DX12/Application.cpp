#include "Application.h"

#include <cassert>

using kitsune::scenegraph::dx12::Application;

Application::Application(kitsune::scenegraph::dx12::Bootstrap *Bootstrap)
	: Bootstrap(Bootstrap)
{
}

Application::~Application()
{
}

void Application::setWindowDimentions(LONG Width, LONG Height)
{
	this->WindowWidth = Width;
	this->WindowHeight = Height;
}

void Application::setWindowTitle(LPCTSTR Title)
{
	if (WindowTitle != nullptr)
		delete[] (LPTCH*)WindowTitle;

	int Size = lstrlen(Title);
	WindowTitle = (LPTSTR)new LPTCH[Size + 1];
	lstrcpy(WindowTitle, Title);
	WindowTitle[Size] = TEXT('\0');
}

void Application::createWindow(HINSTANCE hInstance, int ShowCommand)
{
	WNDCLASSEX WindowClass = { 0 };
	WindowClass.cbSize = sizeof(WNDCLASSEX);
	WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WindowClass.hInstance = hInstance;
	WindowClass.lpszClassName = TEXT("SceneGraph.DX12");
	WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc = WindowProc;
	Bootstrap->setWindowClass(WindowClass);

	RegisterClassEx(&WindowClass);

	RECT WindowRect = { 0, 0, WindowWidth, WindowHeight };
	AdjustWindowRect(&WindowRect, WS_OVERLAPPEDWINDOW, FALSE);

	MainWindow = CreateWindowEx(NULL, 
		WindowClass.lpszClassName,
		getWindowTitle(),
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		WindowRect.right - WindowRect.left,
		WindowRect.bottom - WindowRect.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(MainWindow, ShowCommand);
}

int Application::run()
{
	MSG Message = { 0 };

	while (true) {
		if (PeekMessage(&Message, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);

			if (Message.message == WM_QUIT)
				break;
		}

		Bootstrap->onUpdate();
		Bootstrap->onRender();
	}

	return Message.wParam & 0xFFFF;
}

LRESULT CALLBACK Application::WindowProc(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(Window, Message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	using kitsune::scenegraph::dx12::AppBootstrap;

	assert(AppBootstrap != nullptr && "AppBootstrap is not set");

	Application * App = new Application(AppBootstrap);

	AppBootstrap->onInitializing(App);

	App->createWindow(hInstance, nCmdShow);

	AppBootstrap->onInitialized();

	int retCode = App->run();

	AppBootstrap->onTerminating();

	return retCode;
}
