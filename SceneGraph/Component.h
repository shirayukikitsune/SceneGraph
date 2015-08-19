#pragma once

#include "Node.h"

#include <memory>

namespace kitsune {
namespace scenegraph {

	class Scene;

	class Component
	{
	public:
		Component(std::shared_ptr<Node> Node);
		virtual ~Component();

		bool isLocalActive() const { return Active; }
		bool isActive() const;

	private:
		std::weak_ptr<Node> Node;
		
		std::weak_ptr<Scene> Scene;

		bool Active;
	};

}
}