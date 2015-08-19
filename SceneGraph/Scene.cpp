#include "Scene.h"

#include "Node.h"

using kitsune::scenegraph::Scene;

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::initialize()
{
	onPreInit();

	onInit();
}

void Scene::update(float DeltaTime)
{
}
