#pragma once

#include <map>
#include <memory>

namespace kitsune {
namespace scenegraph {

	class Component;

	class Node
	{
	public:
		Node();
		~Node();

		bool isLocalActive() const { return Active; }
		bool isActive() const;
		void setActive(bool State) { Active = State; }

		void addComponent(std::size_t typehash, Component * Component);
		bool hasComponent(std::size_t typehash);
		Component * getComponent(std::size_t typehash);

		template <class T>
		void addComponent(T * Component) {
			addComponent(T::componentNameHash, Component);
		}
		template <class T>
		bool hasComponent() {
			return hasComponent(T::componentNameHash);
		}
		template <class T>
		T * getComponent() {
			return (T*)getComponent(T::componentNameHash);
		}

	protected:

	private:
		std::weak_ptr<Node> ParentNode;

		std::multimap<std::size_t, std::unique_ptr<Component>> Components;

		bool Active;
	};

}
}

