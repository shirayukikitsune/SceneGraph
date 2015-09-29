#include "Node.h"

#include "Component.h"

using kitsune::scenegraph::Node;
namespace sg = kitsune::scenegraph;

Node::Node(std::weak_ptr<sg::Scene> Scene)
	: Scene(Scene)
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

void Node::addComponent(std::size_t typehash, sg::Component * Component)
{
	if (!Component)
		return;

	Component->setNode(shared_from_this());
	std::unique_ptr<sg::Component> NewComponent(Component);
	Components.emplace(typehash, std::move(NewComponent));
}

bool Node::hasComponent(std::size_t typehash)
{
	return Components.find(typehash) != Components.end();
}

sg::Component * Node::getComponent(std::size_t typehash)
{
	auto &i = Components.find(typehash);

	if (i == Components.end())
		return nullptr;

	return i->second.get();
}

std::shared_ptr<Node> Node::addChildNode()
{
	std::shared_ptr<Node> Node(new Node(this->Scene));

	Node->ParentNode = shared_from_this();

	ChildNodes.emplace(Node);

	invalidate();

	return Node;
}

std::shared_ptr<Node> Node::getParentNode()
{
	if (auto Lock = ParentNode.lock())
		return Lock;

	return std::shared_ptr<Node>();
}

std::shared_ptr<sg::Scene> Node::getScene()
{
	if (auto Scene = this->Scene.lock())
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
