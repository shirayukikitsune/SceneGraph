#pragma once

#include "../Base/Component.h"
#include "../Base/Scene.h"

namespace kitsune::scenegraph {

	class Node;

	class SceneEventComponent :
		public Component
	{
        KIT_SG_COMPONENT(kitsune::scenegraph::SceneEventComponent)

	public:
		enum class AttachedEvents {
			None,
			PreUpdate,
            PostUpdate,
            PrePhysics,
            PostPhysics
		};

		virtual void onPreUpdate(float DeltaTime) {}
        virtual void onPostUpdate(float DeltaTime) {}

		void registerEvents();

		virtual uint32_t getAttachedEvents() { return (uint32_t)AttachedEvents::None; }

	protected:
		Scene::updateCallback::auto_remover_type ScenePreUpdate;
		Scene::updateCallback::auto_remover_type SceneUpdate;
	};

}
