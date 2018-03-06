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

void Scene::addTaggedNode(const std::string & Tag, std::shared_ptr<kitsune::scenegraph::Node> Node)
{
	// We assume that the node isn't being added twice to the map!
	this->TaggedNodes.emplace(Tag, Node);
}

void Scene::removeTaggedNode(const std::string & Tag, std::shared_ptr<kitsune::scenegraph::Node> Node)
{
	// First find the reference
	auto tagRange = this->TaggedNodes.equal_range(Tag);
	for (auto i = tagRange.first; i != tagRange.second; ++i) {
		if (!(Node < i->second.lock()) && !(i->second.lock() < Node)) {
			// This is the requested node, remove it
			this->TaggedNodes.erase(i);
			return;
		}
	}
}
