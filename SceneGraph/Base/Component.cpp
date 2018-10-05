#include "Component.h"

using kitsune::scenegraph::Component;
namespace sg = kitsune::scenegraph;

Component::Component()
        : Active(true) {
}

bool Component::isActive() const {
    if (!isLocalActive()) {
        return false;
    }

    if (auto HasNode = ParentNode.lock()) {
        return HasNode->isActive();
    }

    return true;
}

void Component::setNode(std::weak_ptr<sg::Node> Node) {
    ParentNode = Node;

    if (auto NodePtr = Node.lock()) {
        ParentScene = NodePtr->getScene();
    } else {
        ParentScene.reset();
    }

    onNodeSet();
    onNodeSetEvent(Node);
}

std::shared_ptr<sg::Node> Component::getNode() {
    if (auto Node = ParentNode.lock()) {
        return Node;
    }

    return std::shared_ptr<sg::Node>();
}

std::shared_ptr<sg::Scene> Component::getScene() {
    if (auto Scene = ParentScene.lock()) {
        return Scene;
    }

    return std::shared_ptr<sg::Scene>();
}

Component::onNodeSetCallback::auto_remover_type
Component::addOnNodeSetEvent(onNodeSetCallback::function_type &&function) {
    return onNodeSetEvent.push_auto(function);
}

Component::onNodeSetCallback::auto_remover_type
Component::addOnNodeSetEvent(const onNodeSetCallback::function_type &function) {
    return onNodeSetEvent.push_auto(function);
}
