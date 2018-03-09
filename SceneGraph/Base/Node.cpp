#include "Node.h"

#include "Component.h"
#include "Scene.h"

#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

using kitsune::scenegraph::Node;
namespace sg = kitsune::scenegraph;

Node::Node(std::weak_ptr<sg::Scene> Scene)
    : ParentScene(Scene)
{
	Active = true;
	RecalculateWorld = true;
    resetTransform();
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

glm::mat4 Node::getWorldTransform()
{
	if (RecalculateWorld) {
		if (auto Parent = getParentNode()) {
			WorldTransform = Parent->getWorldTransform() * getLocalTransform();
		}
		else {
			WorldTransform = getLocalTransform();
		}

		RecalculateWorld = false;
	}

	return WorldTransform;
}

void Node::setWorldTransform(const glm::mat4 & transform)
{
	setLocalTransform(getTransformToOrigin() * transform);
}

glm::mat4 Node::getLocalTransform() const
{
    return glm::translate(glm::mat4(), LocalOffset) * glm::toMat4(LocalRotation) * glm::scale(glm::mat4(), LocalScale);
}

void Node::setLocalTransform(const glm::mat4 & transform)
{
    glm::vec3 skew;
    glm::vec4 perspective;

    glm::decompose(transform, LocalScale, LocalRotation, LocalOffset, skew, perspective);

    invalidate();
}

glm::mat4 Node::getTransformToOrigin()
{
	if (auto Parent = getParentNode()) {
		return glm::affineInverse(Parent->getWorldTransform());
	}

	return glm::mat4();
}

glm::vec3 Node::getLocalOffset() const
{
    return LocalOffset;
}

void Node::setLocalOffset(const glm::vec3 & offset)
{
    LocalOffset = offset;
    invalidate();
}

glm::quat Node::getLocalRotation() const
{
    return LocalRotation;
}

void Node::setLocalRotation(const glm::quat & rotation)
{
    LocalRotation = rotation;
    invalidate();
}

void Node::resetTransform()
{
    LocalOffset = glm::vec3(0.0f);
    LocalRotation = glm::quat();
    LocalScale = glm::vec3(1.0f);
	invalidate();
}

void Node::setTag(const std::string & tag)
{
	// Delete old Tag from scene
	auto scene = getScene();

	if (!Tag.empty()) {
		if (scene) {
			scene->removeTaggedNode(Tag, shared_from_this());
		}
	}

	Tag = tag;

	if (!Tag.empty()) {
		// Insert new tag to scene
		if (scene) {
			scene->addTaggedNode(Tag, shared_from_this());
		}
	}
}

void Node::setTag(std::string && tag)
{
	// Delete old Tag from scene
	auto scene = getScene();

	if (!Tag.empty()) {
		if (scene) {
			scene->removeTaggedNode(Tag, shared_from_this());
		}
	}

	Tag = std::move(tag);

	if (!Tag.empty()) {
		// Insert new tag to scene
		if (scene) {
			scene->addTaggedNode(Tag, shared_from_this());
		}
	}
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
