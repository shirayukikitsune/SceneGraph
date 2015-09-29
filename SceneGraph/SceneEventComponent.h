#pragma once

#include "Component.h"
#include "Scene.h"

namespace kitsune {
namespace scenegraph {

	class Node;

	class SceneEventComponent :
		public Component
	{
		KIT_SG_COMPONENT(kitsune::scenegraph::SceneEventComponent);

	public:
		enum class AttachedEvents {
			None,
			PreUpdate,
			Update
		};

		SceneEventComponent();
		virtual ~SceneEventComponent();

		virtual void onPreUpdate(float DeltaTime) {}
		virtual void onUpdate(float DeltaTime) {}

		void registerEvents();

		virtual uint32_t getAttachedEvents() { return (uint32_t)AttachedEvents::None; }

	protected:
		Scene::updateCallback::auto_remover_type ScenePreUpdate;
		Scene::updateCallback::auto_remover_type SceneUpdate;
	};

}
}

