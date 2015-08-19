#include "Node.h"

using kitsune::scenegraph::Node;

Node::Node()
{
	Active = true;
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
