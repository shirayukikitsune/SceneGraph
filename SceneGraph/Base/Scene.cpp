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

	RootNode.reset(new kitsune::scenegraph::Node(shared_from_this()));

	onInit();
}

void Scene::update(float DeltaTime)
{
	onPreUpdate(DeltaTime);

	preUpdateEvents(DeltaTime);

	onUpdate(DeltaTime);

	updateEvents(DeltaTime);
}

void Scene::render()
{
    getRootNode()->render();
}

Scene::updateCallback::auto_remover_type Scene::addPreUpdateEvent(Scene::updateCallback::function_type && function)
{
	return preUpdateEvents.push_auto(function);
}

Scene::updateCallback::auto_remover_type Scene::addPreUpdateEvent(const Scene::updateCallback::function_type & function)
{
	return preUpdateEvents.push_auto(function);
}

Scene::updateCallback::auto_remover_type Scene::addUpdateEvent(Scene::updateCallback::function_type && function)
{
	return updateEvents.push_auto(function);
}

Scene::updateCallback::auto_remover_type Scene::addUpdateEvent(const Scene::updateCallback::function_type & function)
{
	return updateEvents.push_auto(function);
}
