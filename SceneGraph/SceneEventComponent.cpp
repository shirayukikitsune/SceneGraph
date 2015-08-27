#include "SceneEventComponent.h"

using kitsune::scenegraph::SceneEventComponent;

SceneEventComponent::SceneEventComponent(std::shared_ptr<kitsune::scenegraph::Node> Node)
	: Component(Node)
{
}


SceneEventComponent::~SceneEventComponent()
{
}

void SceneEventComponent::registerEvents()
{
	auto Events = getAttachedEvents();
	auto Scene = getScene();

	if (Events & (uint32_t)AttachedEvents::PreUpdate) {
		ScenePreUpdate = Scene->addPreUpdateEvent(std::bind(&SceneEventComponent::onPreUpdate, this, std::placeholders::_1));
	}

	if (Events & (uint32_t)AttachedEvents::Update) {
		SceneUpdate = Scene->addUpdateEvent(std::bind(&SceneEventComponent::onUpdate, this, std::placeholders::_1));
	}
}
