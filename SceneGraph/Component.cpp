#include "Component.h"

using kitsune::scenegraph::Component;
namespace sg = kitsune::scenegraph;

Component::Component(std::shared_ptr<sg::Node> Node)
	: Active(true)
{
	setNode(Node);
}

Component::~Component()
{
}

bool Component::isActive() const
{
	if (!isLocalActive())
		return false;

	if (auto HasNode = Node.lock()) {
		return HasNode->isActive();
	}

	return true;
}

void Component::initialize()
{
}

void Component::setNode(std::weak_ptr<sg::Node> Node)
{
	this->Node = Node;

	if (auto NodePtr = Node.lock())
		this->Scene = NodePtr->getScene();
	else
		this->Scene.reset();

	onNodeSet();
	onNodeSetEvent(Node);
}

std::shared_ptr<sg::Node> Component::getNode()
{
	if (auto Node = this->Node.lock())
		return Node;

	return std::shared_ptr<sg::Node>();
}

std::shared_ptr<sg::Scene> Component::getScene()
{
	if (auto Scene = this->Scene.lock())
		return Scene;

	return std::shared_ptr<sg::Scene>();
}

Component::onNodeSetCallback::auto_remover_type Component::addOnNodeSetEvent(onNodeSetCallback::function_type && function)
{
	return onNodeSetEvent.push_auto(function);
}

Component::onNodeSetCallback::auto_remover_type Component::addOnNodeSetEvent(const onNodeSetCallback::function_type & function)
{
	return onNodeSetEvent.push_auto(function);
}
