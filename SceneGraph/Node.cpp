#include "Node.h"

#include "Component.h"

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

void Node::addComponent(std::size_t typehash, kitsune::scenegraph::Component * Component)
{
	if (!Component)
		return;

	std::unique_ptr<kitsune::scenegraph::Component> NewComponent(Component);
	Components.emplace(typehash, std::move(NewComponent));
}

bool Node::hasComponent(std::size_t typehash)
{
	return Components.find(typehash) != Components.end();
}

kitsune::scenegraph::Component * Node::getComponent(std::size_t typehash)
{
	auto &i = Components.find(typehash);

	if (i == Components.end())
		return nullptr;

	return i->second.get();
}
