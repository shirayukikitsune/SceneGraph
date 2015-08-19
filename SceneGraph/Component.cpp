#include "Component.h"

using kitsune::scenegraph::Component;
namespace sg = kitsune::scenegraph;

Component::Component(std::shared_ptr<sg::Node> Node)
	: Node(Node)
{
	Active = true;
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