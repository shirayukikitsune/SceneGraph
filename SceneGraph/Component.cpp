#include "Component.h"

using kitsune::scenegraph::Component;
namespace sg = kitsune::scenegraph;

Component::Component(std::shared_ptr<sg::Node> Node)
	: Node(Node)
{
	Active = true;

	this->Scene = Node ? Node->getScene() : nullptr;
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
