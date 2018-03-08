#include "Example.h"

// From SceneGraph.DX12
#include <Application.h>

#include <future>
#include <deque>
#include <vector>

using kitsune::scenegraph::dx12::ExampleApplication;

kitsune::scenegraph::dx12::Bootstrap * kitsune::scenegraph::dx12::AppBootstrap = new ExampleApplication;

void ExampleApplication::onInitializing()
{
    auto App = kitsune::scenegraph::dx12::Application::getInstance();

    App->setWindowTitle(TEXT("Kitsune SceneGraph DirectX12 Sample Application"));
	App->setWindowDimentions(1280, 768);
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
