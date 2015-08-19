#include "Scene.h"

#include "Node.h"

using kitsune::scenegraph::Scene;

Scene::Scene()
{
	RootNode.reset(new kitsune::scenegraph::Node);
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
	onPreUpdate(DeltaTime);

	preUpdateEvents(DeltaTime);

	onUpdate(DeltaTime);

	updateEvents(DeltaTime);
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
