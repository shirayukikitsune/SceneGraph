#include "SceneEventComponent.h"

using kitsune::scenegraph::SceneEventComponent;

void SceneEventComponent::registerEvents() {
    auto Events = getAttachedEvents();
    auto Scene = getScene();

    if (Events & (uint32_t) AttachedEvents::PreUpdate) {
        ScenePreUpdate = Scene->addPreUpdateEvent(
                std::bind(&SceneEventComponent::onPreUpdate, this, std::placeholders::_1));
    }

    if (Events & (uint32_t) AttachedEvents::PostUpdate) {
        SceneUpdate = Scene->addUpdateEvent(std::bind(&SceneEventComponent::onPostUpdate, this, std::placeholders::_1));
    }
}
