#include "Example.h"

#include <future>
#include <deque>
#include <vector>

using kitsune::scenegraph::dx12::ExampleApplication;

kitsune::scenegraph::dx12::Bootstrap * kitsune::scenegraph::dx12::AppBootstrap = new ExampleApplication;

void ExampleApplication::onInitializing(kitsune::scenegraph::dx12::Application * app)
{
	app->setWindowTitle(TEXT("Kitsune SceneGraph DirectX12 Sample Application"));
	app->setWindowDimentions(1280, 768);
}

void ExampleApplication::setWindowClass(WNDCLASSEX & WndClass)
{
}

void ExampleApplication::onInitialized()
{
}

void ExampleApplication::onTerminating()
{
}

void ExampleApplication::onUpdate()
{
}

void ExampleApplication::onRender()
{
}
