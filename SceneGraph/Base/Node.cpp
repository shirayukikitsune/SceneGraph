#include "Node.h"

#include "Component.h"

using kitsune::scenegraph::Node;
namespace sg = kitsune::scenegraph;

Node::Node(std::weak_ptr<sg::Scene> Scene)
    : ParentScene(Scene)
{
	LocalTransform.setIdentity();
	Active = true;
	RecalculateWorld = true;
}

Node::~Node()
{
}

bool Node::isActive() const
{
	if (!isLocalActive())
		return false;

	if (auto Parent = this->ParentNode.lock())
		return Parent->isActive();

	return true;
}

void Node::addComponent(std::size_t typehash, std::unique_ptr<sg::Component> && Component)
{
	if (!Component)
		return;

	sg::Component *ptr = Component.get();

	Component->setNode(shared_from_this());
    Components.emplace(typehash, std::move(Component));

	componentAddedEvent(ptr);
}

void Node::addComponent(std::size_t typehash, sg::Component * Component)
{
	std::unique_ptr<sg::Component> NewComponent(Component);
	addComponent(typehash, std::move(NewComponent));
}

bool Node::hasComponent(std::size_t typehash)
{
	return Components.find(typehash) != Components.end();
}

sg::Component * Node::getComponent(std::size_t typehash)
{
    auto i = Components.find(typehash);

	if (i == Components.end())
		return nullptr;

	return i->second.get();
}

void Node::removeComponent(sg::Component * Component, bool Delete)
{
	if (!Component)
		return;

    for (auto i = Components.begin(); i != Components.end(); ++i) {
		if (i->second.get() == Component) {
			if (!Delete)
				i->second.release();

			Components.erase(i);

			componentRemovedEvent(Component);
			break;
		}
	}
}

std::shared_ptr<Node> Node::addChildNode()
{
    std::shared_ptr<Node> CurrentNode(new Node(this->ParentScene));

    CurrentNode->ParentNode = shared_from_this();

    ChildNodes.emplace(CurrentNode);

	invalidate();

    return CurrentNode;
}

std::shared_ptr<Node> Node::getParentNode()
{
	if (auto Lock = ParentNode.lock())
		return Lock;

	return std::shared_ptr<Node>();
}

std::shared_ptr<sg::Scene> Node::getScene()
{
    if (auto Scene = this->ParentScene.lock())
		return Scene;

	return std::shared_ptr<kitsune::scenegraph::Scene>();
}

const btTransform & Node::getWorldTransform()
{
	if (RecalculateWorld) {
		if (auto Parent = getParentNode()) {
			WorldTransform.mult(Parent->getWorldTransform(), getLocalTransform());
		}
		else {
			WorldTransform = getLocalTransform();
		}

		RecalculateWorld = false;
	}

	return WorldTransform;
}

void Node::setWorldTransform(const btTransform & transform)
{
	setLocalTransform(getTransformToOrigin().inverseTimes(transform));
}

btTransform Node::getTransformToOrigin()
{
	if (auto Parent = getParentNode()) {
		return Parent->getWorldTransform().inverse();
	}

	return btTransform::getIdentity();
}

void Node::resetTransform()
{
	LocalTransform.setIdentity();
	invalidate();
}

Node::invalidatedByParentCallback::auto_remover_type Node::addInvalidatedByParentEvent(invalidatedByParentCallback::function_type && function)
{
	return invalidatedByParentEvent.push_auto(function);
}

Node::invalidatedByParentCallback::auto_remover_type Node::addInvalidatedByParentEvent(const invalidatedByParentCallback::function_type & function)
{
	return invalidatedByParentEvent.push_auto(function);
}

Node::componentChangedCallback::auto_remover_type Node::addComponentAddedEvent(componentChangedCallback::function_type && function)
{
	return componentAddedEvent.push_auto(function);
}

Node::componentChangedCallback::auto_remover_type Node::addComponentAddedEvent(const componentChangedCallback::function_type & function)
{
	return componentAddedEvent.push_auto(function);
}

Node::componentChangedCallback::auto_remover_type Node::addComponentRemovedEvent(componentChangedCallback::function_type && function)
{
	return componentRemovedEvent.push_auto(function);
}

Node::componentChangedCallback::auto_remover_type Node::addComponentRemovedEvent(const componentChangedCallback::function_type & function)
{
	return componentRemovedEvent.push_auto(function);
}

void Node::render()
{
    for (auto & Component : Components) {
        Component.second->render();
    }

    for (auto & Child : ChildNodes) {
        Child->render();
    }
}

void Node::invalidate()
{
	if (RecalculateWorld)
		return;

	RecalculateWorld = true;

	for (auto & Child : ChildNodes) {
		Child->invalidatedByParentEvent();
		Child->invalidate();
	}
}
