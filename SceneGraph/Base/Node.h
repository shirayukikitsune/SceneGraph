#pragma once

#include "Callback.h"

#include <map>
#include <memory>
#include <set>

#include <LinearMath/btTransform.h>

namespace kitsune {
namespace scenegraph {

	class Component;
	class Scene;

	class alignas(16) Node
		: public std::enable_shared_from_this<Node>
	{
	public:
		typedef auto_callback<void(void)> invalidatedByParentCallback;
		typedef auto_callback<void(Component*)> componentChangedCallback;

		Node(std::weak_ptr<Scene> Scene = std::weak_ptr<kitsune::scenegraph::Scene>());
		~Node();

		bool isLocalActive() const { return Active; }
		bool isActive() const;
		void setActive(bool State) { Active = State; }

		void addComponent(std::size_t typehash, Component * Component);
		bool hasComponent(std::size_t typehash);
		Component * getComponent(std::size_t typehash);
		void removeComponent(Component * Component, bool Delete = true);

		template <class T, class... ArgTypes>
		T * createComponent(ArgTypes... args) {
			T * ptr = new T(args...);

			addComponent<T>(ptr);

            ptr->initialize();

			return ptr;
		}
		template <class T>
		void addComponent(T * Component) {
			addComponent(T::componentNameHash, std::forward<T*>(Component));
		}
		template <class T>
		bool hasComponent() {
			return hasComponent(T::componentNameHash);
		}
		template <class T>
		T * getComponent() {
			return (T*)getComponent(T::componentNameHash);
		}

		std::shared_ptr<Node> addChildNode();

		std::shared_ptr<Node> getParentNode();

		std::shared_ptr<Scene> getScene();

		const btTransform & getWorldTransform();
		void setWorldTransform(const btTransform & transform);

		const btTransform & getLocalTransform() const { return LocalTransform; }
		void setLocalTransform(const btTransform & transform) { LocalTransform = transform; invalidate(); }

		btTransform getTransformToOrigin();

		btVector3 getLocalOffset() const { return LocalTransform.getOrigin(); }
		void setLocalOffset(const btVector3 & offset) { LocalTransform.setOrigin(offset); invalidate(); }

		btQuaternion getLocalRotation() const { return LocalTransform.getRotation(); }
		void setLocalRotation(const btQuaternion & rotation) { LocalTransform.setRotation(rotation); invalidate(); }

		void resetTransform();

		invalidatedByParentCallback::auto_remover_type addInvalidatedByParentEvent(invalidatedByParentCallback::function_type && function);
		invalidatedByParentCallback::auto_remover_type addInvalidatedByParentEvent(const invalidatedByParentCallback::function_type & function);

		componentChangedCallback::auto_remover_type addComponentAddedEvent(componentChangedCallback::function_type && function);
		componentChangedCallback::auto_remover_type addComponentAddedEvent(const componentChangedCallback::function_type & function);

		componentChangedCallback::auto_remover_type addComponentRemovedEvent(componentChangedCallback::function_type && function);
		componentChangedCallback::auto_remover_type addComponentRemovedEvent(const componentChangedCallback::function_type & function);

	protected:
		void invalidate();

	private:
		btTransform LocalTransform;
		btTransform WorldTransform;
		bool RecalculateWorld;

		std::weak_ptr<Node> ParentNode;
        std::weak_ptr<Scene> ParentScene;

		std::set<std::shared_ptr<Node>> ChildNodes;

		std::multimap<std::size_t, std::unique_ptr<Component>> Components;

		bool Active;

		invalidatedByParentCallback invalidatedByParentEvent;
		componentChangedCallback componentAddedEvent;
		componentChangedCallback componentRemovedEvent;
	};

}
}

