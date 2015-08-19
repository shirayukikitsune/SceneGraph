#pragma once

#include <map>
#include <memory>
#include <set>

namespace kitsune {
namespace scenegraph {

	class Component;

	class Node
		: public std::enable_shared_from_this<Node>
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

		std::shared_ptr<Node> addChildNode();

		std::shared_ptr<Node> getParentNode();

	private:
		std::weak_ptr<Node> ParentNode;

		std::set<std::shared_ptr<Node>> ChildNodes;

		std::multimap<std::size_t, std::unique_ptr<Component>> Components;

		bool Active;
	};

}
}

